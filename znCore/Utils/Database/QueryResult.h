#pragma once

#include "Field.h"

class QueryResult
{
    public:
        QueryResult(uint64 rowCount, uint32 fieldCount)
            : mFieldCount(fieldCount), mRowCount(rowCount) {}

        virtual ~QueryResult() {}

        virtual bool NextRow() = 0;

        typedef std::map<uint32, std::string> FieldNames;

        uint32 GetField_idx(const std::string &name) const
        {
            for(FieldNames::const_iterator iter = GetFieldNames().begin(); iter != GetFieldNames().end(); ++iter)
            {
                if(iter->second == name)
                    return iter->first;
            }
            _ASSERT(false && "unknown field name");
            return uint32(-1);
        }

        Field *Fetch() const { return mCurrentRow; }

        const Field & operator [] (int index) const { return mCurrentRow[index]; }

        const Field & operator [] (const std::string &name) const
        {
            return mCurrentRow[GetField_idx(name)];
        }

        uint32 GetFieldCount() const { return mFieldCount; }
        uint64 GetRowCount() const { return mRowCount; }
        FieldNames const& GetFieldNames() const {return mFieldNames; }

    protected:
        Field *mCurrentRow;
        uint32 mFieldCount;
        uint64 mRowCount;
        FieldNames mFieldNames;
};
