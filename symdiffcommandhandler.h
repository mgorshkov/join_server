#pragma once

#include "commandhandler.h"

class SymmetricDifferenceCommandHandler : public CommandHandler
{
public:
    SymmetricDifferenceCommandHandler(ITableManager* aTableManager);

    virtual std::string GetCommand() const override;
    virtual CompleteCommand Parse(const std::string& aLine) override;
    CompleteOperationStatus Handle(const CompleteCommand& aCommand) override;
};
