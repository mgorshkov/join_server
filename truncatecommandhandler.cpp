#include <iostream>

#include "truncatecommandhandler.h"

TruncateCommandHandler::TruncateCommandHandler(ITableManager* aTableManager)
    : CommandHandler(aTableManager)
{		
}

std::string TruncateCommandHandler::GetCommand() const
{
    return "TRUNCATE";
}

CompleteCommand TruncateCommandHandler::Parse(const std::string& aLine)
{
    std::string tableName = aLine;

//#ifdef DEBUG_PRINT
    std::cout << "TruncateCommandHandler::Parse, tableName=" << tableName << std::endl;
//#endif

    return CompleteCommand{Command::Truncate, tableName};
}

CompleteOperationStatus TruncateCommandHandler::Handle(const CompleteCommand& aCommand)
{
    return mTableManager->Truncate(aCommand.mTableName);
}

