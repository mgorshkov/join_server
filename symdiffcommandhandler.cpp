#include "symdiffcommandhandler.h"

SymmetricDifferenceCommandHandler::SymmetricDifferenceCommandHandler(ITableManager* aTableManager)
    : CommandHandler(aTableManager)
{		
}

std::string SymmetricDifferenceCommandHandler::GetCommand() const
{
    return "SYMMETRIC_DIFFERENCE";
}

CompleteCommand SymmetricDifferenceCommandHandler::Parse(const std::string&)
{
    return CompleteCommand{Command::SymmetricDifference};
}

CompleteOperationStatus SymmetricDifferenceCommandHandler::Handle(const CompleteCommand&)
{
    return mTableManager->SymmetricDifference();
}

