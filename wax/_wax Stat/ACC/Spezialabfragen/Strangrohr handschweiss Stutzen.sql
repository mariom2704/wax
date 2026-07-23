SELECT  s.wanr, s.waid,
        avelement.dn AS ElemDN,
		avelement.anschlussdn AS RohrDN,
		avelement.anzahl * werkauftrag.anzahl AS WNStutzen,
		0 AS Rohstutzen,
		0 AS HandschwMuffe,
		winkel
FROM  status s, werkauftrag, avelement
where werkauftrag.waid =  s.waid
and avelement.waid = werkauftrag.waid
and avelement.id = 2
and s.wanr in
(

)

SELECT  s.wanr, s.waid,
        avelement.dn AS ElemDN,
		avelement.anschlussdn AS RohrDN,
		0 AS WNStutzen,
        avelement.anzahl * werkauftrag.anzahl AS Rohstutzen,
		0 AS HandschwMuffe,
		winkel
FROM   status s, werkauftrag, avelement
where  werkauftrag.waid =  s.waid
and avelement.waid = werkauftrag.waid
and (avelement.id = 1 or avelement.id = 4)
and s.wanr in
(
)

SELECT  s.wanr, s.waid,
        avelement.dn AS ElemDN,
		avelement.anschlussdn AS RohrDN,
		0 AS WNStutzen,
		0 AS Rohrstutzen,
		avelement.anzahl * werkauftrag.anzahl AS handschwMuff,
		winkel
FROM  status s, werkauftrag, avelement
where werkauftrag.waid =  s.waid
and avelement.waid = werkauftrag.waid
and avelement.id = 5
and s.wanr in
(
);