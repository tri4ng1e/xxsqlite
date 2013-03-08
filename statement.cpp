/*
  Copyright (C) 2013  Nick Ogden <nick@nickogden.org>
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the
  
  Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "statement.h"

#include <sqlite3.h>

#include <ostream>
#include <sstream>
#include <cassert>

namespace sqlite {

int find_parameter_index(sqlite3_stmt *stmt, const std::string &parameter) {
    assert(stmt && "null sqlite3_stmt provided");
    const int index(sqlite3_bind_parameter_index(stmt, parameter.c_str()));
    if (! index) {
        std::stringstream ss;
        ss << "unknown parameter '" << parameter << "' in sql statment '"
           << sqlite3_sql(stmt) << "'";
        throw  database_error(ss.str());
    }
    return index;
}

void throw_on_error(const int status, const std::string &parameter) {
    if (status != SQLITE_OK) {
        std::stringstream ss;
        ss << sqlite3_errstr(status) << " while binding parameter '"
           << parameter << "'";
        throw database_error(ss.str());
    }
}

statement::statement(sqlite3_stmt *statement): stmt(statement) {
    assert(stmt && "attempt to create statement with null sqlite3_stmt");
}

statement::statement(statement &&other) {
    assert(&other != this && "move into same object");
    std::swap(stmt, other.stmt);
}

statement::~statement() {
    sqlite3_finalize(stmt); // Return value is irrelevant
}

statement& statement::operator=(statement &&other) {
    assert(&other != this && "assignment to same object");
    std::swap(stmt, other.stmt);
    return *this;
}

void statement::bind(const std::string &parameter, const blob &value) {
    assert(false && "blob support not implemented");
}

void statement::bind(const std::string &parameter, const double value) {
    const int index(find_parameter_index(stmt, parameter));
    throw_on_error(sqlite3_bind_double(stmt, index, value), parameter);
}

void statement::bind(const std::string &parameter, const int value) {
    const int index(find_parameter_index(stmt, parameter));
    throw_on_error(sqlite3_bind_int(stmt, index, value), parameter);
}

void statement::bind(const std::string &parameter, const int64_t value) {
    const int index(find_parameter_index(stmt, parameter));
    throw_on_error(sqlite3_bind_int64(stmt, index, value), parameter);
}

void statement::bind(const std::string &parameter, const null_value value) {
    const int index(find_parameter_index(stmt, parameter));
    throw_on_error(sqlite3_bind_null(stmt, index), parameter);
}

void statement::bind(const std::string &parameter, const std::string &value) {
    const int index(find_parameter_index(stmt, parameter));
    const int status(sqlite3_bind_text(
        stmt, index, value.c_str(), value.size(), SQLITE_STATIC
    ));
    throw_on_error(status, parameter);
}

std::ostream & operator<<(std::ostream &os, const statement &statement) {
    os << "statement: (display not implemented)\n";
    return os;
}

} // namespace sqlite
