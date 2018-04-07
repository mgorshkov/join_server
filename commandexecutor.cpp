#include <assert.h>

#include "commandexecutor.h"
#include "insertcommandhandler.h"
#include "truncatecommandhandler.h"
#include "intersectcommandhandler.h"
#include "symdiffcommandhandler.h"

CommandExecutor::CommandExecutor(ITableManager* aTableManager)
{
	RegisterHandler(Command::Insert, std::make_shared<InsertCommandHandler>(aTableManager));
    RegisterHandler(Command::Truncate, std::make_shared<TruncateCommandHandler>(aTableManager));
    RegisterHandler(Command::Intersection, std::make_shared<IntersectionCommandHandler>(aTableManager));
    RegisterHandler(Command::SymmetricDifference, std::make_shared<SymmetricDifferenceCommandHandler>(aTableManager));
}

CompleteOperationStatus CommandExecutor::RunCommand(const std::string& aLine)
{
    CompleteCommand command = Parse(aLine);
    return mCommandHandlers[command.mCommand]->Handle(command);
}

void CommandExecutor::RegisterHandler(Command aCommand, std::shared_ptr<CommandHandler> aCommandHandler)
{
	assert (mCommandHandlers.find(aCommand) == mCommandHandlers.end());
	mCommandHandlers[aCommand] = aCommandHandler;
}

CompleteCommand CommandExecutor::Parse(const std::string& aLine)
{
	for (const auto& handler: mCommandHandlers)
	{
		auto pos = aLine.find(handler.second->GetCommand());
		if (pos != std::string::npos)
			return handler.second->Parse(aLine);
	}

	return CompleteCommand{Command::Error};
}
