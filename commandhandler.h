#pragma once

#include "itablemanager.h"
#include "structs.h"

class CommandHandler
{
public:
    CommandHandler(ITableManager* aTableManager);

    virtual ~CommandHandler() = default;

    virtual std::string GetCommand() const = 0;
    virtual CompleteCommand Parse(const std::string& aLine) = 0;
    virtual CompleteOperationStatus Handle(const CompleteCommand& aCommand) = 0;

protected:
    ITableManager* mTableManager;
};
