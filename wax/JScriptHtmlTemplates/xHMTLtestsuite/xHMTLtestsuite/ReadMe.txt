================================================================================
    MICROSOFT FOUNDATION CLASS-BIBLIOTHEK: 
    xHMTLtestsuite-Projektübersicht
===============================================================================

Der Anwendungs-Assistent hat diese xHMTLtestsuite-Anwendung für Sie 
erstellt. Diese Anwendung zeigt nicht nur die Grundlagen der Verwendung von 
Microsoft Foundation Classes, sondern dient auch als Ausgangspunkt für das 
Schreiben Ihrer Anwendung.

Diese Datei bietet eine Übersicht über den Inhalt der einzelnen Dateien, aus
denen Ihre xHMTLtestsuite-Anwendung besteht.

xHMTLtestsuite.vcproj
    Dies ist die Hauptprojektdatei für VC++-Projekte, die mithilfe eines 
    Anwendungs-Assistenten erstellt werden.
    Sie enthält Informationen zur Visual C++-Version, mit der die Datei 
    generiert wurde, sowie Informationen zu Plattformen, Konfigurationen und 
    Projektfeatures, die mit dem Anwendungs-Assistenten ausgewählt wurden.

xHMTLtestsuite.h
    Dies ist die Hauptheaderdatei für die Anwendung. Sie enthält weitere
    projektspezifische Header (einschließlich "Resource.h") und deklariert
    die CxHMTLtestsuiteApp-Anwendungsklasse.

xHMTLtestsuite.cpp
    Dies ist die Quelldatei der Hauptanwendung, die die CxHMTLtestsuiteApp-
    Anwendungsklasse enthält.

xHMTLtestsuite.rc
    Dies ist eine Auflistung aller vom Programm verwendeten 
    Microsoft Windows-Ressourcen. Sie enthält die Symbole, Bitmaps und Cursor, 
    die im Unterverzeichnis "RES" gespeichert werden. Diese Datei kann direkt 
    in Microsoft Visual C++ bearbeitet werden. Ihre Projektressourcen befinden 
    sich in 1031.

res\xHMTLtestsuite.ico
    Dies ist eine Symboldatei, die als Symbol der Anwendung verwendet wird. 
    Dieses Symbol ist in der Hauptressourcendatei "xHMTLtestsuite.rc" 
    enthalten.

res\xHMTLtestsuite.rc2
    Diese Datei enthält Ressourcen, die nicht von Microsoft Visual C++ 
    bearbeitet werden. Sie sollten alle Ressourcen, die nicht mit dem
    Ressourcen-Editor bearbeitet werden können, in dieser Datei platzieren.

/////////////////////////////////////////////////////////////////////////////

Für das Hauptrahmenfenster:
    Das Projekt enthält eine standardmäßige MFC-Schnittstelle.

MainFrm.h, MainFrm.cpp
    Diese Datei enthält die CMainFrame-Rahmenklasse, die von
    CMDIFrameWnd abgeleitet wird und alle MDI-Rahmenfunktionen steuert.

res\Toolbar.bmp
    Diese Bitmapdatei wird verwendet, um gekachelte Bilder für die Symbolleiste 
    zu erstellen.
    Die ursprüngliche Symbolleiste und Statusleiste werden in der 
    CMainFrame-
    Klasse erstellt. Bearbeiten Sie diese Symbolleistenbitmap mithilfe des
    Ressourcen-Editors, und aktualisieren Sie das IDR_MAINFRAME TOOLBAR-Array
    in "xHMTLtestsuite.rc", um Symbolleisten-Schaltflächen
    hinzuzufügen.
/////////////////////////////////////////////////////////////////////////////

Für das untergeordnete Rahmenfenster:

ChildFrm.h, ChildFrm.cpp
    Diese Dateien definieren und implementieren die CChildFrame-
    Klasse, die die untergeordneten Fenster in einer MDI-Anwendung unterstützt.

/////////////////////////////////////////////////////////////////////////////

Der Anwendungs-Assistent erstellt einen Dokumenttyp und eine Ansicht:

xHMTLtestsuiteDoc.h, xHMTLtestsuiteDoc.cpp – das Dokument
    Diese Dateien enthalten Ihre CxHMTLtestsuiteDoc-Klasse. Bearbeiten Sie 
    diese Dateien, um spezielle Dokumentdaten hinzuzufügen und Dateispeicherung
    und -ladung (über CxHMTLtestsuiteDoc::Serialize) zu implementieren.

xHMTLtestsuiteView.h, xHMTLtestsuiteView.cpp – die Ansicht des Dokuments
    Diese Dateien enthalten Ihre CxHMTLtestsuiteView-Klasse.
    CxHMTLtestsuiteView-Objekte werden verwendet, um 
    CxHMTLtestsuiteDoc-Objekte anzuzeigen.

res\xHMTLtestsuiteDoc.ico
    Dies ist eine Symboldatei, die als Symbol für untergeordnete MDI-Fenster
    für die CxHMTLtestsuiteDoc-Klasse verwendet wird. Dieses Symbol ist in der 
    Hauptressourcendatei "xHMTLtestsuite.rc" enthalten.




/////////////////////////////////////////////////////////////////////////////

Weitere Funktionen:

ActiveX-Steuerelemente
    Die Anwendung unterstützt die Verwendung von ActiveX-Steuerelementen.

Druck- und Seitenansichtunterstützung
    Der Anwendungs-Assistent hat Code generiert, um die Befehle für Drucken, 
    Druckeinrichtung und Seitenansicht zu verarbeiten, indem Memberfunktionen 
    in der CView-Klasse aus der MFC-Bibliothek aufgerufen werden.

/////////////////////////////////////////////////////////////////////////////

Andere Standarddateien:

StdAfx.h, StdAfx.cpp
    Mit diesen Dateien werden eine vorkompilierte Headerdatei (PCH)
    mit dem Namen "xHMTLtestsuite.pch" und eine 
    vorkompilierte Typendatei mit dem Namen "StdAfx.obj" erstellt.

Resource.h
    Dies ist die Standardheaderdatei, die neue Ressourcen-IDs definiert.
    Die Datei wird mit Microsoft Visual C++ gelesen und aktualisiert.

xHMTLtestsuite.manifest
	Anwendungsmanifestdateien werden in Windows XP zur Beschreibung einer
	Anwendungsabhängigkeit von verschiedenen Versionen paralleler 
        Assemblys verwendet. 
	Das Ladeprogramm verwendet diese Informationen, um die entsprechende 
	Assembly aus dem Assemblycache oder privat aus der Anwendung zu laden.
	Das Anwendungsmanifest kann zur Verteilung als externe Manifestdatei 
        enthalten sein, die im gleichen Ordner installiert ist wie die 
        ausführbare Datei der Anwendung, oder sie kann in Form einer Ressource 
        in der ausführbaren Datei enthalten sein.
/////////////////////////////////////////////////////////////////////////////

Weitere Hinweise:

Der Anwendungs-Assistent verwendet "TODO:", um auf Teile des Quellcodes
hinzuweisen, die Sie ergänzen oder anpassen sollten.

Wenn Ihre Anwendung MFC in einer gemeinsam genutzten DLL verwendet, 
müssen Sie die MFC-DLLs verteilen. Wenn die Anwendung eine andere Sprache 
als die des Gebietsschemas des Betriebssystems verwendet, müssen Sie außerdem 
die entsprechenden lokalisierten Ressourcen in "MFC90XXX.DLL" verteilen.
Weitere Informationen zu diesen beiden Themen finden Sie im Abschnitt 
zum Verteilen von Visual C++-Anwendungen in der Dokumentation zu MSDN.

/////////////////////////////////////////////////////////////////////////////
