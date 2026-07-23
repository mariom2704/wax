SELECT  s.wanr, s.waid,
       IF ( eleid = 2, vk3listewa.dn, 0 ) AS ElemDN,
		IF ( eleid = 2, vk3listewa.anschlussdn, 0 ) AS RohrDN,
		IF ( eleid = 2, anzahllaenge, 0) AS WNStutzen,
		0 AS Rohstutzen,
		0 AS HandschwMuffe													
FROM  vk3listewa, status s, werkauftrag
where vk3listewa.waid = s.waid 
and werkauftrag.waid =  s.waid
and s.wanr in
(
)
UNION
SELECT  s.wanr, s.waid,
        IF ( eleid = 1   OR eleid = 4, vk3listewa.dn, 0 ) AS ElemDN,
		IF ( eleid = 1 OR eleid = 4, vk3listewa.anschlussdn, 0 ) AS RohrDN,
		0 AS WNStutzen,
        IF ( eleid = 1  OR  eleid = 4, anzahllaenge, 0) AS Rohstutzen,
		0 AS HandschwMuffe	
FROM  vk3listewa, status s, werkauftrag
where vk3listewa.waid = s.waid 
and werkauftrag.waid =  s.waid
and s.wanr in
(
)
UNION
SELECT  s.wanr, s.waid,
        IF ( eleid = 5, vk3listewa.dn, 0 ) AS ElemDN,
		IF (eleid = 5, vk3listewa.anschlussdn, 0 ) AS RohrDN,
		0 AS WNStutzen,
		0 AS Rohrstutzen,
		IF (eleid = 5, anzahllaenge, 0) AS handschwMuff   
FROM  vk3listewa, status s, werkauftrag
where vk3listewa.waid = s.waid 
and werkauftrag.waid =  s.waid
and s.wanr in
(
);

