#include "node.h"
#include "context.h"
#include "error.h"



Node::Node() { 
    throw GeneralException(); 
}

Node::Node(const Node &rhs) : 
    number(rhs.number), context(rhs.context), bw(rhs.bw), lat(rhs.lat) {}




Node & Node::operator=(const Node &rhs) 
{
    return *(new(this)Node(rhs));
}



/*************** set and get for members ***************/

void Node::SetNumber(const unsigned n) 
{ 
    number=n;
}

unsigned Node::GetNumber() const 
{ 
    return number;
}

void Node::SetLatency(const double l)
{ 
    lat=l;
}

double Node::GetLatency() const 
{ 
    return lat;
}

void Node::SetBW(const double b)
{ 
    bw=b;
}

double Node::GetBW() const 
{ 
    return bw;
}

Node::~Node()
{}

// Implement these functions  to post an event to the event queue in the event simulator
// so that the corresponding node can recieve the ROUTING_MESSAGE_ARRIVAL event at the proper time
void Node::SendToNeighbors(const RoutingMessage *m)
{
    //cerr<<"from node: "<<GetNumber()<<endl;
    /*
    for(deque<Node*>::iterator i = (context->GetNeighbors(this)).begin(); i != (context->GetNeighbors(this)).end();i++){
        cerr<<(**i).GetNumber()<<" "<<endl;
    }
    */
    context->SendToNeighbors(this,m);
}

void Node::SendToNeighbor(const Node *n, const RoutingMessage *m)
{
    context->SendToNeighbor(this,n,m);
}

deque<Node*> *Node::GetNeighbors()
{
    return context->GetNeighbors(this);
}

void Node::SetTimeOut(const double timefromnow)
{
    context->TimeOut(this,timefromnow);
}


bool Node::Matches(const Node &rhs) const
{
    return number==rhs.number;
}   


#if defined(GENERIC)
void Node::LinkHasBeenUpdated(const Link *l)
{
    cerr << *this << " got a link update: "<<*l<<endl;
    //Do Something generic:
    SendToNeighbors(new RoutingMessage);
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
    cerr << *this << " got a routing messagee: "<<*m<<" Ignored "<<endl;
}


void Node::TimeOut()
{
    cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
    return 0;
}

Table *Node::GetRoutingTable() const
{
    return new Table;
}


ostream & Node::Print(ostream &os) const
{
    os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
    return os;
}

Node::Node(const unsigned n, SimulationContext *c, double b, double l) :
    number(n), context(c), bw(b), lat(l) {}

#endif















/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/







#if defined(LINKSTATE)


Node::Node(const unsigned n, SimulationContext *c, double b, double l) : 
    number(n), context(c), bw(b), lat(l) 
{
    if (b!=0 || l!=0){
        table = new Table(n);
        //cerr<<"node added with num: "<<n<<endl;
    }
}

void Node::LinkHasBeenUpdated(const Link *l)
{
    //cerr << *this<<": Link Update: "<<*l<<endl;

    //extract link data
    unsigned linkSrc = l->GetSrc();
    unsigned linkDest = l->GetDest();
    double linkLat = l->GetLatency();
    //build table
    vector<Entry> *firstRow = new vector<Entry>();
    Entry * first = new Entry(linkSrc, linkSrc, 0, false);
    Entry * second = new Entry(linkDest, linkSrc, linkLat, false);
    (*firstRow).push_back(*first);
    (*firstRow).push_back(*second);
    vector<vector<Entry> > * newDataLine = new vector<vector<Entry> >();
    (*newDataLine).push_back(*firstRow);

    //update line
    bool success = table->LSUpdateLine(newDataLine);

    //send out routing messages
    if (success == true)
    {
        RoutingMessage * outMessage = new RoutingMessage(*newDataLine, true);
        //table->PrintCerr();
        SendToNeighbors(outMessage);
    }
    //table->PrintCerr();
    table->CalculateDResult();

}



void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
    //cerr << *this << " Routing Message: "<<m->linkChange<<endl;
    const vector<vector<Entry> > * t = &(m->neighborMessage);

    bool success;
    //m->linkChange is both true or false, do calculateDResult.
    //update table
    if (m->linkChange == false)
    {
        success = table->LSUpdateTable(&(m->neighborMessage));
    }
    //only update link
    else
    {
        success = table->LSUpdateLine(&(m->neighborMessage));
    }

    //cerr<<"success:  "<<success<<endl;

    //table or line has been updated, send out routing messages
    
    if (success == true)
    {
        RoutingMessage * outMessage = new RoutingMessage((*table->tableData), false);
        SendToNeighbors(outMessage);
        
    }
    
    
    table->CalculateDResult();
}


void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
    // WRITE
    // desNumber is the Destination number of the Node destination
    // myNumber is the number of this Node
    // getPredNumber is the number of Previous Node of getRightEntry
    unsigned desNumber = destination->GetNumber();
    unsigned myNumber = GetNumber();
    unsigned getPredNumber = desNumber;
    Entry* getRightEntry = table->DGetEntry(getPredNumber);
    while (1)
    {
        //if getRightEntry = NULL, namely the entry containning the getPredNumber doesn't
        //exit, return NULL
        // or the Entry's pred number is itself, return NULL
        if ((getRightEntry == NULL)||(getRightEntry->GetPred() == getPredNumber))
        {
            return NULL;
        }
        //if the Entry's pred number is myNumber, the right entry was found
        else if (getRightEntry->GetPred() == myNumber)
        {
            Node * resultNode = new Node(getPredNumber,context,0,0);
            return resultNode;
        }
        //
        else
        {
            getPredNumber = getRightEntry->GetPred();
            getRightEntry = table->DGetEntry(getPredNumber);
        }
    }
}


Table *Node::GetRoutingTable() const
{
    Table *copytable = new Table(*table);
    return copytable;
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
  return os;
}
#endif









/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/






#if defined(DISTANCEVECTOR)

Node::Node(const unsigned n, SimulationContext *c, double b, double l) : 
    number(n), context(c), bw(b), lat(l) 
{
    if (b!=0 || l!=0){
        table = new Table(n);
        //cerr<<"node added with num: "<<n<<endl;
    }
}



void Node::LinkHasBeenUpdated(const Link *l)
{
    // update our table
    // send out routing mesages
    //cerr << *this<<" :Link Update: Src="<<l->GetSrc()<<" Dest="<<l->GetDest()<<" Latency="<<l->GetLatency()<<endl;
    //table->PrintCerr();
    if (table->GetLinePlus(l->GetDest())) {
        //If this link has already been added
        table->UpdateTable(l->GetDest(),l->GetLatency());
        //(table->GetEntryPlus(table->Get1stLine(),l->GetDest()))->SetCost(l->GetLatency());
    }
    else{
        //If this link has not been added yet
        //cerr<<"add new link: "<<l->GetSrc()<<"->"<<l->GetDest()<<" "<<l->GetLatency()<<endl;
        table->CreateLine((*l).GetDest());
        Entry newEntry(l->GetDest(),l->GetDest(),l->GetLatency());
        (table->Get1stLine())->push_back(newEntry);
    }   
    RoutingMessage * outMessage = new RoutingMessage(*(table->Get1stLine()));
    
    SendToNeighbors(outMessage);

    //table->PrintCerr(); 
}

 


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
    /*
    cerr << *this << " Routing Message: "<<endl;
    for(int i=0;i<m->neighborMessage.size();i++){
        cerr<<((m->neighborMessage)[i]).GetNumber()<<"/"<<((m->neighborMessage)[i]).GetCost()<<" ";
    }
    cerr << endl;
    */
    vector<Entry> * changeLineNumber = table->ChangeLine(m->neighborMessage, (m->neighborMessage)[0].GetNumber());
    //cerr << "Already ChangeLine"<<endl;
    //table->PrintCerr(); 

    bool sendChange = table->UpdateTable(changeLineNumber); //cerr << "Already

    if (sendChange == true){
        //the first row info of this router
        vector<Entry> * firstLine = table->Get1stLine();

        //------not sure about Parameters------
        RoutingMessage * outMessage = new RoutingMessage(*firstLine);
        
        SendToNeighbors(outMessage);
    }
    //table->PrintCerr();
}

void Node::TimeOut()
{
    cerr << *this << " got a timeout: ignored"<<endl;
}


Node *Node::GetNextHop(const Node *destination) const
{
    // WRITE
    //get the table
    unsigned destination_number = destination->GetNumber();
    unsigned result = table->GetEntryPlus(table->Get1stLine(), destination_number)->GetNextHop();
    Node * resultNode = new Node(result,context,0,0);
    return resultNode;

    
}

Table *Node::GetRoutingTable() const
{   
    Table *copytable = new Table(*table);
    return copytable;
}


ostream & Node::Print(ostream &os) const
{
    os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw;
    return os;
}
#endif
