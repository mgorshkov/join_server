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
	auto pos = aLine.find(' ');
	if (pos == std::string::npos)
		return CompleteCommand{Command::Error};

	int id = std::atoi(aLine.substr(0, pos).c_str());
	return CompleteCommand{Command::Insert};
}

CompleteOperationStatus TruncateCommandHandler::Handle(const CompleteCommand& aCommand)
{
	return mTableManager->Truncate(aCommand.mTableName);
}

