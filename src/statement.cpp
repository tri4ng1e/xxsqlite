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
#include "error.h"
#include "parameter_lookup.h"

#include <sqlite3.h>

#include <ostream>
#include <sstream>
#include <cassert>

namespace sqlite {

statement::statement(const std::shared_ptr<sqlite3_stmt> &statement):
        stmt(statement) {
    assert(statement && "attempt to create statement with null sqlite3_stmt");
}

void statement::bind(const std::string &parameter, const blob &value) {
    assert(false && "blob support not implemented");
}

void statement::bind(const std::string &parameter, const double &value) {
    auto index(internal::find_parameter_index(parameter, stmt));
    throw_on_bind_error(sqlite3_bind_double(stmt.get(), index, value), parameter);
}

void statement::bind(const std::string &parameter, const int &value) {
    auto index(internal::find_parameter_index(parameter, stmt));
    throw_on_bind_error(sqlite3_bind_int(stmt.get(), index, value), parameter);
}

void statement::bind(const std::string &parameter, const int64_t &value) {
    auto index(internal::find_parameter_index(parameter, stmt));
    throw_on_bind_error(sqlite3_bind_int64(stmt.get(), index, value), parameter);
}

void statement::bind(const std::string &parameter, const null_t &value) {
    (void) value;
    auto index(internal::find_parameter_index(parameter, stmt));
    throw_on_bind_error(sqlite3_bind_null(stmt.get(), index), parameter);
}

void statement::bind(const std::string &parameter, const std::string &value) {
    auto index(internal::find_parameter_index(parameter, stmt));
    auto status(sqlite3_bind_text(
        stmt.get(), index, value.c_str(), value.size(), SQLITE_STATIC
    ));
    throw_on_bind_error(status, parameter);
}

void statement::clear_bindings() {
    assert(stmt && "clear_bindings() called with null sqlite3_stmt");
    auto status(sqlite3_clear_bindings(stmt.get()));
    if (status != SQLITE_OK)
        throw error(status);
}

result statement::execute() {
    assert(stmt && "execute() called with null sqlite3_stmt");
    (void) sqlite3_reset(stmt.get());
    return stmt;
}

std::size_t statement::parameter_count() const {
    return sqlite3_bind_parameter_count(stmt.get());
}

void statement::throw_on_bind_error(
        const int status,
        const std::string &parameter
) const {
    if (status != SQLITE_OK)
        throw bind_error(parameter, stmt);
}

std::ostream& operator<<(std::ostream &os, const statement &statement) {
    os << "statement:\n"
          "  sql: " << sqlite3_sql(statement.stmt.get()) << "\n";
    return os;
}

} // namespace sqlite