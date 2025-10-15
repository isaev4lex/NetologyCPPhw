#pragma once
#include "allinclude.h"

class SqlSelectQueryBuilder {
private:
    std::vector<std::string> columns_;
    std::string from_table_;
    std::vector<std::pair<std::string, std::string>> where_;

public:
    SqlSelectQueryBuilder& AddColumn(const std::string& column) noexcept {
        columns_.push_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) noexcept {
        columns_.insert(columns_.end(), columns.begin(), columns.end());
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) noexcept {
        from_table_ = table;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& value) noexcept {
        where_.push_back({ key, value });
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
        for (const auto& [key, value] : kv) {
            where_.push_back({ key, value });
        }
        return *this;
    }

    std::string BuildQuery() const {
        std::ostringstream oss;

        // select
        if (columns_.empty()) {
            oss << "SELECT *";
        }
        else {
            oss << "SELECT ";
            for (size_t i = 0; i < columns_.size(); ++i) {
                oss << columns_[i];
                if (i != columns_.size() - 1) oss << ", ";
            }
        }

        //from
        if (!from_table_.empty()) {
            oss << " FROM " << from_table_;
        }

        //where
        if (!where_.empty()) {
            oss << " WHERE ";
            for (size_t i = 0; i < where_.size(); ++i) {
                oss << where_[i].first << "=" << where_[i].second;
                if (i != where_.size() - 1) oss << " AND ";
            }
        }

        oss << ";";
        return oss.str();
    }
};
