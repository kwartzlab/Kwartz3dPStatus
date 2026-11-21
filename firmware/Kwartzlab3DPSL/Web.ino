AsyncWebServer WebServer(80);

void WebActionManualUpdate(AsyncWebServerRequest *request) {
  if(!request->hasParam("printer", true) || !request->hasParam("status", true)) {
      Serial.println("manualupdate.action: missing required parameter(s)");
      request->redirect("/tools.html");
      return;
  }

  if(request->getParam("status", true)->value() == "claimed") {
    PrinterUpdateClaimed(request->getParam("printer", true)->value().toInt(), true);
  } else {
    PrinterUpdateState(atoi(request->getParam("printer", true)->value().c_str()), request->getParam("status", true)->value().c_str());
  }

  request->redirect("/tools.html");
}

void WebActionLedTest(AsyncWebServerRequest *request) {
  if(!request->hasParam("led", true) || !request->hasParam("colour", true)) {
      Serial.println("ledtest.action: missing required parameter(s)");
      request->redirect("/tools.html");
      return;
  }
  
  String colourString = request->getParam("colour", true)->value();
  
  uint8_t red, green, blue;
  red = colourString.substring(0, colourString.indexOf(',')).toInt();
  Serial.println(colourString.substring(0, colourString.indexOf(',')));
  Serial.println(red);
  
  green = colourString.substring(colourString.indexOf(',')+1, colourString.lastIndexOf(',')).toInt();
  Serial.println(colourString.substring(colourString.indexOf(',')+1, colourString.lastIndexOf(',')));
  Serial.println(green);

  blue = colourString.substring(colourString.lastIndexOf(',')+1).toInt();
  Serial.println(colourString.substring(colourString.lastIndexOf(',')+1));
  Serial.println(blue);

  LedSetColour(atoi(request->getParam("led", true)->value().c_str()), red, green, blue);

  request->redirect("/tools.html");
}

// TODO: actually use the mappings!
void WebActionMappings(AsyncWebServerRequest *request) {
  char keyName[9];

  for(uint8_t printerNbr = 1; printerNbr<10; printerNbr++) {
    sprintf(keyName, "printer%d", printerNbr);
    if(!request->hasParam(keyName, true)) {
      Serial.printf("mappings.action: missing required parameter(s) '%s'", keyName);
      return;
    }
    uint8_t led = atoi(request->getParam(keyName, true)->value().c_str());
    if(PrinterGetMapping(printerNbr) != led)
      PrinterSetMapping(printerNbr, led);
  }

  request->redirect("/config.html");
}

String WebTemplateIndex(const String &var) {
  if(var.startsWith("PRINTER") && var.endsWith("LED")) {
    uint8_t printerNbr = var.charAt(7)-48; // convert ASCII character to integer
    uint8_t led = PrinterGetMapping(printerNbr); 
    if(led == PRINTER_NOT_PRESENT)
      return String("Not Present");
    return String("LED " + String(led));
  } else if(var.startsWith("PRINTER") and var.endsWith("STATE")) {
    uint8_t printerNbr = var.charAt(7)-48; // convert ASCII character to integer
    uint8_t led = PrinterGetMapping(printerNbr);
    if(led == PRINTER_NOT_PRESENT)
      return asyncsrv::emptyString;
    // TODO: add claim status
    switch(PrinterGetState(printerNbr)) {
      case STATE_IDLE:
        return String(": Idle");
      case STATE_PRINTING:
        if(PrinterGetClaimed(printerNbr))
          return String(": Printing, claimed");
        else
          return String(": Printing, unclaimed");
      case STATE_PAUSED:
        if(PrinterGetClaimed(printerNbr))
          return String(": Paused, claimed");
        else
          return String(": Paused, unclaimed");
      case STATE_ERROR:
        if(PrinterGetClaimed(printerNbr))
          return String(": Error, claimed");
        else
          return String(": Error, unclaimed");
      default:
        return String(": Unknown State");
    }
  }

  return String("UNKNOWN PARAMETER");
}

String WebTemplateConfig(const String &var) {
  if(var.startsWith("PRINTER") && var.indexOf("LED") != -1) {
    // - 48 converts ASCII character to integer
    uint8_t printerNbr = var.charAt(7)-48; 
    uint8_t ledNbr = var.charAt(11)-48;
    if(PrinterGetMapping(printerNbr) == ledNbr) {
      return String("selected");
    } else {
      return asyncsrv::emptyString;
    }
  }

  return String("UNKNOWN PARAMETER");
}

void WebInit(void) {
  if (!LittleFS.begin(false)) {
    Serial.println("ERROR: LittleFS mount failed, web interface will not be started.");
    return;
  }

  WebServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->redirect("/index.html");
  });

  // set up the static pages that serve right from the filesystem
  // TODO: do this programmatically rather than hardcoding it all
  WebServer.serveStatic("/3dpsl.css", LittleFS, "/3dpsl.css");
  WebServer.serveStatic("/logo.svg", LittleFS, "/logo.svg");

  WebServer.serveStatic("/index.html", LittleFS, "/index.html").setTemplateProcessor(WebTemplateIndex);
  WebServer.serveStatic("/config.html", LittleFS, "/config.html").setTemplateProcessor(WebTemplateConfig);
  WebServer.serveStatic("/tools.html", LittleFS, "/tools.html");

  // set up the "CGI" stuff
  WebServer.on("/manualupdate.action", HTTP_POST, WebActionManualUpdate);
  WebServer.on("/ledtest.action", HTTP_POST, WebActionLedTest);
  WebServer.on("/mappings.action", HTTP_POST, WebActionMappings);

  WebServer.begin();
}
