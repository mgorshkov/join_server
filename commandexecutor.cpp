#include <assert.h>
#include <iostream>

#include "commandexecutor.h"
#include "insertcommandhandler.h"
#include "truncatecommandhandler.h"
#include "intersectcommandhandler.h"
#include "symdiffcommandhandler.h"

CommandExecutor::CommandExecutor(ITableManager* aTableManager)
{
    RegisterHandler(Command::Insert, std::make_unique<InsertCommandHandler>(aTableManager));
    RegisterHandler(Command::Truncate, std::make_unique<TruncateCommandHandler>(aTableManager));
    RegisterHandler(Command::Intersection, std::make_unique<IntersectionCommandHandler>(aTableManager));
    RegisterHandler(Command::SymmetricDifference, std::make_unique<SymmetricDifferenceCommandHandler>(aTableManager));
}

CompleteOperationStatus CommandExecutor::RunCommand(const std::string& aLine)
{
//#ifdef DEBUG_PRINT
    std::cout << "CommandExecutor::RunCommand, line=" << aLine << ";" << std::endl;
//#endif
    CompleteCommand command = Parse(aLine);
//#ifdef DEBUG_PRINT
    std::cout << "CommandExecutor::RunCommand, command=" << command << ";" << std::endl;
//#endif
    return mCommandHandlers[command.mCommand]->Handle(command);
}

void CommandExecutor::RegisterHandler(Command aCommand, std::unique_ptr<CommandHandler> aCommandHandler)
{
    assert (mCommandHandlers.find(aCommand) == mCommandHandlers.end());
    mCommandHandlers.emplace(std::make_pair(aCommand, std::move(aCommandHandler)));
}

CompleteCommand CommandExecutor::Parse(const std::string& aLine)
{
    std::cout << "CommandExecutor::Parse, aLine=" << aLine << ";" << std::endl;

    int size = mCommandHandlers.size();

    std::cout << "CommandExecutor::Parse, aLine=" << aLine << ";" << size << std::endl;

    for (const auto& handler: mCommandHandlers)
    {
        auto pos = aLine.find(handler.second->GetCommand());
//#ifdef DEBUG_PRINT
        std::cout << "CommandExecutor::Parse, pos=" << pos << ";" << std::endl;
//#endif
        if (pos != std::string::npos)
            return handler.second->Parse(aLine);
    }   
    
    return CompleteCommand{Command::Error};
}
