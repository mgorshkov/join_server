#include "symdiffcommandhandler.h"

SymmetricDifferenceCommandHandler::SymmetricDifferenceCommandHandler(ITableManager* aTableManager)
	: CommandHandler(aTableManager)
{		
}

std::string SymmetricDifferenceCommandHandler::GetCommand() const
{
	return "SYMMETRIC_DIFFERENCE";
}

CompleteCommand SymmetricDifferenceCommandHandler::Parse(const std::string& aLine)
{
	auto pos = aLine.find(' ');
	if (pos == std::string::npos)
		return CompleteCommand{Command::Error};

	int id = std::atoi(aLine.substr(0, pos).c_str());
	return CompleteCommand{Command::Insert};
}

CompleteOperationStatus SymmetricDifferenceCommandHandler::Handle(const CompleteCommand&)
{
	return mTableManager->SymmetricDifference();
}

