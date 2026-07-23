SELECT 'Summe CAD' AS SumPos, sum(werkauftrag.anzahl) AS Anzahl_Rohre FROM  Position, werkauftrag, status  where status.waid = Position.waid AND Position.waid = werkauftrag.waid  

and  status.freigabedatum >= *WaxDateVon*
and  status.freigabedatum <= *WaxDateBis*
and 
not IsNull(Position.Verzeichnis)

UNION
SELECT  'Summe Wax' AS SumPos, sum(werkauftrag.anzahl) AS Anzahl_Rohre  FROM werkauftrag, status where status.waid = werkauftrag.waid

and  status.freigabedatum >= *WaxDateVon*
and  status.freigabedatum <= *WaxDateBis*
and werkauftrag.waid not in
(SELECT Position.waid FROM  status, Position where  
	
status.freigabedatum >= *WaxDateVon*
and  status.freigabedatum <= *WaxDateBis*

and status.waid = Position.waid)
