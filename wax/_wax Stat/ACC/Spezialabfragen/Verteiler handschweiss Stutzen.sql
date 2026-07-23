SELECT  s.wanr, s.waid,
        element.dn AS ElemDN,
		element.anschlussdn AS RohrDN,
		element.anzahl * werkauftrag.anzahl AS WNStutzen,
		0 AS Rohstutzen,
		0 AS HandschwMuffe,
		winkel
FROM  status s, werkauftrag, element
where werkauftrag.waid =  s.waid
and element.waid = werkauftrag.waid
and element.id = 2
and s.wanr in
(
)
UNION
SELECT  s.wanr, s.waid,
        element.dn AS ElemDN,
		element.anschlussdn AS RohrDN,
		0 AS WNStutzen,
        element.anzahl * werkauftrag.anzahl AS Rohstutzen,
		0 AS HandschwMuffe,
		winkel
FROM   status s, werkauftrag, element
where  werkauftrag.waid =  s.waid
and element.waid = werkauftrag.waid
and (element.id = 1 or element.id = 4)
and s.wanr in
()
UNION
SELECT  s.wanr, s.waid,
         element.dn AS ElemDN,
		element.anschlussdn AS RohrDN,
		0 AS WNStutzen,
		0 AS Rohrstutzen,
		element.anzahl * werkauftrag.anzahl AS handschwMuff,
		winkel
FROM  status s, werkauftrag, element
where werkauftrag.waid =  s.waid
and element.waid = werkauftrag.waid
and element.id = 5
and s.wanr in
(
);