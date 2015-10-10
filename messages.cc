#include "messages.h"


#if defined(GENERIC)
ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage()";
  return os;
}
#endif


#if defined(LINKSTATE)

ostream &RoutingMessage::Print(ostream &os) const
{
  return os;
}

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(const vector<vector<Entry> > (& nM), const bool l):
neighborMessage(nM), linkChange(l)
{
	
}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs):
neighborMessage(rhs.neighborMessage), linkChange(rhs.linkChange)
{}

#endif


#if defined(DISTANCEVECTOR)

ostream &RoutingMessage::Print(ostream &os) const
{
  return os;
}

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(const vector<Entry> (& nM)):
neighborMessage(nM)
{
	
}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs):
neighborMessage(rhs.neighborMessage)
{}

#endif

