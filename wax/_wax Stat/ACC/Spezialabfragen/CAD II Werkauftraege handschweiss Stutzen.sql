SELECT  s.wanr, s.waid,
        Anschluss.DnId AS ElemDN,
		TypRohr.DnId AS RohrDN,
		werkauftrag.anzahl AS WNStutzen,
		0 AS Rohstutzen,
		0 AS HandschwMuffe,
		WaxWinkel
FROM  status s, werkauftrag, BauteilPos, Anschluss, TypRohr
where werkauftrag.waid =  s.waid
and BauteilPos.waid =  werkauftrag.waid
and Anschluss.waid =  werkauftrag.waid
and werkauftrag.waid = TypRohr.Waid
and BauteilPos.Id = Anschluss.AnschlussId
and BauteilPos.Artikelgruppe = 2
and s.wanr in
(
)



SELECT  s.wanr, s.waid, Anschluss.Id, AnschlussId,
        Anschluss.DnId AS ElemDN,
		TypRohr.DnId AS  RohrDN,
		0 AS WNStutzen,
       	werkauftrag.anzahl AS Rohstutzen,
		0 AS HandschwMuffe,
		WaxWinkel
FROM   status s, werkauftrag, BauteilPos, Anschluss, TypRohr
where werkauftrag.waid =  s.waid
and BauteilPos.waid =  werkauftrag.waid
and Anschluss.waid =  werkauftrag.waid
and werkauftrag.waid = TypRohr.Waid
and BauteilPos.Id = Anschluss.AnschlussId
and WaxWinkel > 0
and (BauteilPos.Artikelgruppe = 51) 
and s.wanr in
(
)



SELECT  s.wanr, s.waid,
        Anschluss.DnId AS ElemDN,
		TypRohr.DnId AS RohrDN,
		0 AS WNStutzen,
		0 AS Rohstutzen,
		werkauftrag.anzahl AS handschwMuff,
		WaxWinkel
FROM  status s, werkauftrag, BauteilPos, Anschluss, TypRohr
where werkauftrag.waid =  s.waid
and BauteilPos.waid =  werkauftrag.waid
and Anschluss.waid =  werkauftrag.waid
and werkauftrag.waid = TypRohr.Waid
and BauteilPos.Id = Anschluss.AnschlussId
and BauteilPos.Artikelgruppe = 5
and s.wanr in
(
);