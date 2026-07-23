#include "stdafx.h"
#include "convxml2wax.h"


using namespace DOM;

class MemParseHandlers : public HandlerBase
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    MemParseHandlers();
    ~MemParseHandlers();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    unsigned int getElementCount()
    {
        return fElementCount;
    }

    unsigned int getAttrCount()
    {
        return fAttrCount;
    }

    unsigned int getCharacterCount()
    {
        return fCharacterCount;
    }

    unsigned int getSpaceCount()
    {
        return fSpaceCount;
    }


    // -----------------------------------------------------------------------
    //  Handlers for the SAX DocumentHandler interface
    // -----------------------------------------------------------------------
    void startElement(const XMLCh* const name, DOM::AttributeList& att);
   void resetDocument();


    // -----------------------------------------------------------------------
    //  Handlers for the SAX ErrorHandler interface
    // -----------------------------------------------------------------------
    void warning(const DOM::SAXParseException& exc);
    void error(const DOM::SAXParseException& exc);
    void fatalError(const DOM::SAXParseException& exc);



private:
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fAttrCount
    //  fCharacterCount
    //  fElementCount
    //  fSpaceCount
    //      These are just counters that are run upwards based on the input
    //      from the document handlers.
    // -----------------------------------------------------------------------
    unsigned int    fAttrCount;
    unsigned int    fCharacterCount;
    unsigned int    fElementCount;
    unsigned int    fSpaceCount;
};

XERCES_STD_QUALIFIER ostream& operator<<(XERCES_STD_QUALIFIER ostream& target, const XStr& toDump)
{
	target << toDump.to_string();
	TRACE("%s\n", toDump.to_string().substr(0, 255).c_str());
    return target;
}

// ---------------------------------------------------------------------------
//  MemParseHandlers: Constructors and Destructor
// ---------------------------------------------------------------------------
MemParseHandlers::MemParseHandlers() :
    fAttrCount(0)
    , fCharacterCount(0)
    , fElementCount(0)
    , fSpaceCount(0)
{
}

MemParseHandlers::~MemParseHandlers()
{
}


// ---------------------------------------------------------------------------
//  MemParseHandlers: Implementation of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void MemParseHandlers::startElement(const   XMLCh* const   name 
                                    ,       AttributeList&  att)
{
    fElementCount++;
    fAttrCount += att.getLength();
	XStr str(name);
	TRACE("%s\n", str.to_string().substr(0,255).c_str());
	for (int i = 0; i < att.getLength(); i++)
		TRACE("-> %s = %s\n", XStr(att.getName(i)).to_string().substr(0,255).c_str(), XStr(att.getValue(i)).to_string().substr(0,255).c_str());
}


void MemParseHandlers::resetDocument()
{
    fAttrCount = 0;
    fCharacterCount = 0;
    fElementCount = 0;
    fSpaceCount = 0;
}



// ---------------------------------------------------------------------------
//  MemParseHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void MemParseHandlers::error(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nError at (file " << XStr(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "): " << XStr(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void MemParseHandlers::fatalError(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nFatal Error at (file " << XStr(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "): " << XStr(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}

void MemParseHandlers::warning(const SAXParseException& e)
{
    XERCES_STD_QUALIFIER cerr << "\nWarning at (file " << XStr(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "): " << XStr(e.getMessage()) << XERCES_STD_QUALIFIER endl;
}



ConvXml2Wax::ConvXml2Wax()
{
}

ConvXml2Wax::~ConvXml2Wax()
{
}

bool ConvXml2Wax::convert(CadWerkauftrag* rohr)
{
	MxXml::DomDocument doc; // Initialisiert und terminiert die XML Unterstützung

	const string& xml = rohr->get_xml_modell();
	MemParseHandlers handler;
	doc.parse(xml, handler);
    
	return true;
}

