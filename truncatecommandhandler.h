#pragma once

#include "commandhandler.h"
#include "itablemanager.h"

class TruncateCommandHandler : public CommandHandler
{
public:
    TruncateCommandHandler(ITableManager* aTableManager);

    virtual std::string GetCommand() const override;
    virtual CompleteCommand Parse(const std::string& aLine) override;
    virtual CompleteOperationStatus Handle(const CompleteCommand& aCommand) override;
};
