/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _PREPAREDSTATEMENT_H
#define _PREPAREDSTATEMENT_H

#include "Define.h"
#include "Duration.h"
#include "SQLOperation.h"
#include <tuple>
#include <future>
#include <vector>
#include <variant>

namespace Trinity::Types
{
    template <typename T>
    using is_default = std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<std::vector<uint8>, T>>;

    template <typename T>
    using is_enum_v = std::enable_if_t<std::is_enum_v<T>>;

    template <typename T>
    using is_non_string_view_v = std::enable_if_t<!std::is_base_of_v<std::string_view, T>>;
}

struct PreparedStatementData
{
    std::variant<
        bool,
        uint8,
        uint16,
        uint32,
        uint64,
        int8,
        int16,
        int32,
        int64,
        float,
        double,
        std::string,
        std::vector<uint8>,
        SystemTimePoint,
        std::nullptr_t
    > data;

    template<typename T>
    static std::string ToString(T value);

    static std::string ToString(bool value);
    static std::string ToString(uint8 value);
    static std::string ToString(int8 value);
    static std::string ToString(std::string const& value);
    static std::string ToString(std::vector<uint8> const& value);
    static std::string ToString(SystemTimePoint value);
    static std::string ToString(std::nullptr_t);
};

//- Upper-level class that is used in code
class TC_DATABASE_API PreparedStatementBase
{
    friend class PreparedStatementTask;

    public:
        explicit PreparedStatementBase(uint32 index, uint8 capacity);
        virtual ~PreparedStatementBase();

        void setNull(uint8 index);
        void setBool(uint8 index, bool value);
        void setUInt8(uint8 index, uint8 value);
        void setUInt16(uint8 index, uint16 value);
        void setUInt32(uint8 index, uint32 value);
        void setUInt64(uint8 index, uint64 value);
        void setInt8(uint8 index, int8 value);
        void setInt16(uint8 index, int16 value);
        void setInt32(uint8 index, int32 value);
        void setInt64(uint8 index, int64 value);
        void setFloat(uint8 index, float value);
        void setDouble(uint8 index, double value);
        void setDate(uint8 index, SystemTimePoint value);
        void setString(uint8 index, std::string const& value);
        void setStringView(uint8 index, std::string_view value);
        void setBinary(uint8 index, std::vector<uint8> const& value);
        template <size_t Size>
        void setBinary(const uint8 index, std::array<uint8, Size> const& value)
        {
            std::vector<uint8> vec(value.begin(), value.end());
            setBinary(index, vec);
        }

        // Set numerlic and default binary
        template<typename T>
        inline Trinity::Types::is_default<T> SetData(const uint8 index, T value)
        {
            SetValidData(index, value);
        }

        // Set enums
        template<typename T>
        inline Trinity::Types::is_enum_v<T> SetData(const uint8 index, T value)
        {
            SetValidData(index, std::underlying_type_t<T>(value));
        }

        // Set string_view
        inline void SetData(const uint8 index, std::string_view value)
        {
            SetValidData(index, value);
        }

        // Set nullptr
        inline void SetData(const uint8 index, std::nullptr_t = nullptr)
        {
            SetValidData(index);
        }

        // Set non default binary
        template<std::size_t Size>
        inline void SetData(const uint8 index, std::array<uint8, Size> const& value)
        {
            std::vector<uint8> vec(value.begin(), value.end());
            SetValidData(index, vec);
        }

        // Set duration
        template<class _Rep, class _Period>
        inline void SetData(const uint8 index, std::chrono::duration<_Rep, _Period> const& value, bool convertToUin32 = true)
        {
            SetValidData(index, convertToUin32 ? static_cast<uint32>(value.count()) : value.count());
        }

        // Set all
        template <typename... Args>
        inline void SetArguments(Args&&... args)
        {
            SetDataTuple(std::make_tuple(std::forward<Args>(args)...));
        }

        uint32 GetIndex() const { return m_index; }
        std::vector<PreparedStatementData> const& GetParameters() const { return statement_data; }

    protected:
        template<typename T>
        Trinity::Types::is_non_string_view_v<T> SetValidData(const uint8 index, T const& value);

        void SetValidData(const uint8 index);
        void SetValidData(const uint8 index, std::string_view value);

        template<typename... Ts>
        inline void SetDataTuple(std::tuple<Ts...> const& argsList)
        {
            std::apply
            (
                [this](Ts const&... arguments)
                {
                    uint8 index{ 0 };
                    ((SetData(index, arguments), index++), ...);
                }, argsList
            );
        }
        uint32 m_index;

        //- Buffer of parameters, not tied to MySQL in any way yet
        std::vector<PreparedStatementData> statement_data;

        PreparedStatementBase(PreparedStatementBase const& right) = delete;
        PreparedStatementBase& operator=(PreparedStatementBase const& right) = delete;
};

template<typename T>
class PreparedStatement : public PreparedStatementBase
{
public:
    explicit PreparedStatement(uint32 index, uint8 capacity) : PreparedStatementBase(index, capacity)
    {
    }

private:
    PreparedStatement(PreparedStatement const& right) = delete;
    PreparedStatement& operator=(PreparedStatement const& right) = delete;
};

//- Lower-level class, enqueuable operation
class TC_DATABASE_API PreparedStatementTask : public SQLOperation
{
    public:
        PreparedStatementTask(PreparedStatementBase* stmt, bool async = false);
        ~PreparedStatementTask();

        bool Execute() override;
        PreparedQueryResultFuture GetFuture() { return m_result->get_future(); }

    protected:
        PreparedStatementBase* m_stmt;
        bool m_has_result;
        PreparedQueryResultPromise* m_result;
};
#endif
