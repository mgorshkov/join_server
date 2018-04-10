#pragma once

#include "commandhandler.h"

class IntersectionCommandHandler : public CommandHandler
{
public:
    IntersectionCommandHandler(ITableManager* aTableManager);

    virtual std::string GetCommand() const override;
    virtual CompleteCommand Parse(const std::string& aLine) override;
    virtual CompleteOperationStatus Handle(const CompleteCommand& aCommand) override;
};

