#pragma once

#include <map>
#include <memory>

#include "itablemanager.h"
#include "structs.h"
#include "commandhandler.h"

class CommandExecutor
{
public:
    CommandExecutor(ITableManager* aTableManager);

    CompleteOperationStatus RunCommand(const std::string& aLine);

private:
	void RegisterHandler(Command aCommand, std::shared_ptr<CommandHandler> aCommandHandler);
    CompleteCommand Parse(const std::string& aLine);

    std::map<Command, std::shared_ptr<CommandHandler>> mCommandHandlers;
};
