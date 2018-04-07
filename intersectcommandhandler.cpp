#include "intersectcommandhandler.h"

IntersectionCommandHandler::IntersectionCommandHandler(ITableManager* aTableManager)
	: CommandHandler(aTableManager)
{		
}

std::string IntersectionCommandHandler::GetCommand() const
{
	return "INTERSECTION";
}

CompleteCommand IntersectionCommandHandler::Parse(const std::string& aLine)
{
	auto pos = aLine.find(' ');
	if (pos == std::string::npos)
		return CompleteCommand{Command::Error};

	int id = std::atoi(aLine.substr(0, pos).c_str());
	return CompleteCommand{Command::Insert};
}

CompleteOperationStatus IntersectionCommandHandler::Handle(const CompleteCommand&)
{
	return mTableManager->Intersection();
}

