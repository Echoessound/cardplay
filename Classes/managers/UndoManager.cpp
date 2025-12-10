#include "managers/UndoManager.h"

void UndoManager::pushRecord(const UndoRecord& record)
{
    _records.push_back(record);
}

bool UndoManager::popRecord(UndoRecord& outRecord)
{
    if (_records.empty())
    {
        return false;
    }
    outRecord = _records.back();
    _records.pop_back();
    return true;
}

void UndoManager::clear()
{
    _records.clear();
}

bool UndoManager::hasRecord() const
{
    return !_records.empty();
}



