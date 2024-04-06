#include "node.h"

Node::Node(const BPMN::Model& model, XML::bpmn::tCollaboration& collaboration)
 : id(collaboration.id.value().get().value.value)
 , type("Collaboration")
 , documentation( 
     collaboration.getOptionalChild<XML::bpmn::tDocumentation>().has_value() ? 
     collaboration.getOptionalChild<XML::bpmn::tDocumentation>().value().get().textContent : 
     ""
   )
{
//std::cerr << type << std::endl;
  for ( auto& process : model.processes ) {
    processes.emplace_back(Node(process.get()));
  }
}

Node::Node(BPMN::Process* process)
 : id(process->id)
 , type("Process")
 , documentation( 
     process->element->getOptionalChild<XML::bpmn::tDocumentation>().has_value() ?
     process->element->getOptionalChild<XML::bpmn::tDocumentation>().value().get().textContent :
     ""
   )
 , extensionElements(
     process->extensionElements && process->extensionElements->element ?
     process->extensionElements->element->format() :
     ""
   )
{
//std::cerr << type << std::endl;
  for ( auto& messageFlow : process->sending ) {
    messageFlows.emplace_back(Node(messageFlow));
  }
  for ( auto& dataObject : process->dataObjects ) {
    dataObjects.emplace_back(Node(dataObject.get()));
  }
  for ( auto& flowNode : process->flowNodes ) {
    flowNodes.emplace_back(Node(flowNode));
  }
  for ( auto& eventSubProcess : process->eventSubProcesses ) {
    eventSubProcesses.emplace_back(Node(eventSubProcess));
  }
}

Node::Node(BPMN::EventSubProcess* eventSubProcess)
 : id(eventSubProcess->id)
 , type("Event-SubProcess")
 , documentation( 
     eventSubProcess->element->getOptionalChild<XML::bpmn::tDocumentation>().has_value() ?
     eventSubProcess->element->getOptionalChild<XML::bpmn::tDocumentation>().value().get().textContent :
     ""
   )
 , extensionElements(
     eventSubProcess->extensionElements && eventSubProcess->extensionElements->element ?
     eventSubProcess->extensionElements->element->format() :
     ""
   )
{
//std::cerr << type << std::endl;
  for ( auto& dataObject : eventSubProcess->dataObjects ) {
    dataObjects.emplace_back(Node(dataObject.get()));
  }
  for ( auto& flowNode : eventSubProcess->flowNodes ) {
    flowNodes.emplace_back(Node(flowNode));
  }
  for ( auto& eventSubProcess : eventSubProcess->eventSubProcesses ) {
    eventSubProcesses.emplace_back(Node(eventSubProcess));
  }
}

Node::Node(BPMN::FlowNode* flowNode)
 : id(flowNode->id)
 , type(getType(flowNode))
 , documentation(
     flowNode->element->getOptionalChild<XML::bpmn::tDocumentation>().has_value() ?
     flowNode->element->getOptionalChild<XML::bpmn::tDocumentation>().value().get().textContent :
     ""
   )
 , extensionElements(
     flowNode->extensionElements && flowNode->extensionElements->element ?
     flowNode->extensionElements->element->format() :
     ""
   )
{
//std::cerr << type << std::endl;
  for ( auto& sequenceFlow : flowNode->outgoing ) {
    sequenceFlows.emplace_back(Node(sequenceFlow));
  }
  for ( auto& messageFlow : flowNode->sending ) {
    messageFlows.emplace_back(Node(messageFlow));
  }

  if ( auto scope = flowNode->represents<BPMN::Scope>() ) {
    for ( auto& dataObject : scope->dataObjects ) {
      dataObjects.emplace_back(Node(dataObject.get()));
    }
    for ( auto& flowNode : scope->flowNodes ) {
      flowNodes.emplace_back(Node(flowNode));
    }
    for ( auto& eventSubProcess : scope->eventSubProcesses ) {
      eventSubProcesses.emplace_back(Node(eventSubProcess));
    }
  }
}

Node::Node(BPMN::DataObject* dataObject)
 : id(dataObject->id)
 , type("DataObject")
 , documentation(
     dataObject->element->getOptionalChild<XML::bpmn::tDocumentation>().has_value() ?
     dataObject->element->getOptionalChild<XML::bpmn::tDocumentation>().value().get().textContent :
     ""
   )
 , extensionElements(
     dataObject->extensionElements && dataObject->extensionElements->element ?
     dataObject->extensionElements->element->format() :
     ""
   )
{
//std::cerr << type << std::endl;
}

Node::Node(BPMN::SequenceFlow* sequenceFlow)
 : id(sequenceFlow->id)
 , type("Outgoing SequenceFlow")
 , documentation(
     sequenceFlow->element->getOptionalChild<XML::bpmn::tDocumentation>().has_value() ?
     sequenceFlow->element->getOptionalChild<XML::bpmn::tDocumentation>().value().get().textContent :
     ""
   )
 , extensionElements(
     sequenceFlow->extensionElements && sequenceFlow->extensionElements->element ?
     sequenceFlow->extensionElements->element->format() :
     ""
   )
{
//std::cerr << type << std::endl;
}

Node::Node(BPMN::MessageFlow* messageFlow)
 : id(messageFlow->id)
 , type("Outgoing MessageFlow")
 , documentation(
     messageFlow->element->getOptionalChild<XML::bpmn::tDocumentation>().has_value() ?
     messageFlow->element->getOptionalChild<XML::bpmn::tDocumentation>().value().get().textContent :
     ""
   )
 , extensionElements(
     messageFlow->extensionElements && messageFlow->extensionElements->element ?
     messageFlow->extensionElements->element->format() :
     ""
   )
{
//std::cerr << type << std::endl;
}

std::string Node::getType(BPMN::FlowNode* flowNode) {
  if ( flowNode->represents<BPMN::SubProcess>() ) {
    return "SubProcess";
  } 
  else if ( flowNode->represents<BPMN::AdHocSubProcess>() ) {
    return "Ad-hoc SubProcess";
  } 
  else if ( flowNode->represents<BPMN::CallActivity>() ) {
    return "Call Activity";
  } 
  else if ( flowNode->represents<BPMN::Task>() ) {
    return "Task";
  } 
  else if ( flowNode->represents<BPMN::Gateway>() ) {
    return "Gateway";
  } 
  else if ( flowNode->represents<BPMN::Event>() ) {
    return "Event";
  } 
  return "Other";
}

