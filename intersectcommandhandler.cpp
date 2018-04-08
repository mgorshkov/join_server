#include "intersectcommandhandler.h"

IntersectionCommandHandler::IntersectionCommandHandler(ITableManager* aTableManager)
    : CommandHandler(aTableManager)
{		
}

std::string IntersectionCommandHandler::GetCommand() const
{
    return "INTERSECTION";
}

CompleteCommand IntersectionCommandHandler::Parse(const std::string&)
{
    return CompleteCommand{Command::Intersection};
}

CompleteOperationStatus IntersectionCommandHandler::Handle(const CompleteCommand&)
{
    return mTableManager->Intersection();
}

