#pragma once
#include <unordered_map>
#include <string>
#include <any>
#include <optional>
#include <mutex>
#include <iostream>

class MagicVault {
public:
    // Проксирующая структура для доступа vault["Group"]["Key"] = value;
    class GroupProxy {
    public:
        GroupProxy(MagicVault& vault, const std::string& group)
            : vault(vault), groupName(group) {}

        // set
        template<typename T>
        void set(const std::string& key, T value) {
            vault.set(groupName, key, std::any(value));
        }

        // convenience: assignment-like
        template<typename T>
        void assign(const std::string& key, T value) {
            set<T>(key, std::move(value));
        }

        // get optional
        template<typename T>
        std::optional<T> getOpt(const std::string& key) const {
            return vault.getOpt<T>(groupName, key);
        }

        // get with default (throws bad_any_cast if wrong type)
        template<typename T>
        T get(const std::string& key, T defaultValue = T()) const {
            auto opt = getOpt<T>(key);
            if (opt.has_value()) return *opt;
            return defaultValue;
        }

        bool has(const std::string& key) const {
            return vault.has(groupName, key);
        }

        void remove(const std::string& key) {
            vault.remove(groupName, key);
        }

    private:
        MagicVault& vault;
        std::string groupName;
    };

    // Получить proxy для группы: vault.group("LoginGate") или operator[]
    GroupProxy group(const std::string& groupName) {
        return GroupProxy(*this, groupName);
    }

    GroupProxy operator[](const std::string& groupName) {
        return group(groupName);
    }

    // Прямые методы (thread-safe)
    template<typename T>
    void set(const std::string& group, const std::string& key, T value) {
        std::lock_guard<std::mutex> g(mutex_);
        storage_[group][key] = std::any(std::move(value));
    }

    bool has(const std::string& group, const std::string& key) const {
        std::lock_guard<std::mutex> g(mutex_);
        auto itg = storage_.find(group);
        if (itg == storage_.end()) return false;
        return itg->second.find(key) != itg->second.end();
    }

    template<typename T>
    std::optional<T> getOpt(const std::string& group, const std::string& key) const {
        std::lock_guard<std::mutex> g(mutex_);
        auto itg = storage_.find(group);
        if (itg == storage_.end()) return std::nullopt;
        auto itk = itg->second.find(key);
        if (itk == itg->second.end()) return std::nullopt;
        try {
            return std::any_cast<T>(itk->second);
        } catch (const std::bad_any_cast&) {
            return std::nullopt; // wrong type
        }
    }

    template<typename T>
    T getOr(const std::string& group, const std::string& key, T defaultVal) const {
        auto opt = getOpt<T>(group, key);
        if (opt.has_value()) return *opt;
        return defaultVal;
    }

    void remove(const std::string& group, const std::string& key) {
        std::lock_guard<std::mutex> g(mutex_);
        auto itg = storage_.find(group);
        if (itg == storage_.end()) return;
        itg->second.erase(key);
    }

    void clearGroup(const std::string& group) {
        std::lock_guard<std::mutex> g(mutex_);
        storage_.erase(group);
    }

    void clearAll() {
        std::lock_guard<std::mutex> g(mutex_);
        storage_.clear();
    }

    // Debug print (types are printed by type().name())
    void dump(std::ostream& os = std::cout) const {
        std::lock_guard<std::mutex> g(mutex_);
        for (const auto& [grp, mapkv] : storage_) {
            os << "[" << grp << "]\n";
            for (const auto& [k, v] : mapkv) {
                os << "  " << k << " : " << v.type().name() << "\n";
            }
        }
    }

    // Singleton access
    static MagicVault& Instance() {
        static MagicVault inst;
        return inst;
    }

private:
    MagicVault() = default;
    ~MagicVault() = default;
    MagicVault(const MagicVault&) = delete;
    MagicVault& operator=(const MagicVault&) = delete;

    mutable std::mutex mutex_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::any>> storage_;
};
