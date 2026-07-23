SELECT p.nr as PANr, 

(select max(auftragsbez) from waxserver.werkauftrag where p.nr = preisanfrageNr) as Auftrag,
(select max(sdauftragsnr) from waxserver.werkauftrag where p.nr = preisanfrageNr) as 'SD-Nr',
(select max(kcauftragsnr) from waxserver.werkauftrag where p.nr = preisanfrageNr) as 'KC-Nr',

(select max(nl) from waxserver.werkauftrag w where p.nr = w.preisanfrageNr) as Nl,
(select bezeichnung from waxstamm.niederlassungen where nlnummer =  (select max(nl) from waxserver.werkauftrag w where p.nr = w.preisanfrageNr) ) as NlBez,
(select region from waxstamm.niederlassungen where nlnummer =  (select max(nl) from waxserver.werkauftrag w where p.nr = w.preisanfrageNr) ) as Region,

(select max(sachbearbeiternl) from waxserver.werkauftrag w where p.nr = w.preisanfrageNr ) as SachbearbeiterNl,

sum(preis) as PA_Preis,

(SELECT sum(v.preis * v.anzahllaenge)
FROM waxserver.vk3listewa v  where waid = (select w.waid from waxserver.werkauftrag w where w.waid=v.waid and w.preisanfrageNr =  p.nr)
group by (select w.preisanfrageNr from waxserver.werkauftrag w where w.waid=v.waid and w.preisanfrageNr =  p.nr)) as vk3Preis,

(100 - (100 * sum(preis) /
(SELECT sum(v.preis * v.anzahllaenge)
FROM waxserver.vk3listewa v  where waid = (select w.waid from waxserver.werkauftrag w where w.waid=v.waid and w.preisanfrageNr =  p.nr)
group by (select w.preisanfrageNr from waxserver.werkauftrag w where w.waid=v.waid and w.preisanfrageNr =  p.nr)))) as Nachlass,


(select max(s.freigabedatum) from waxserver.status s where waid =  (select max(waid) from waxserver.werkauftrag w where p.nr = w.preisanfrageNr)) as Freigabedatum,

(select max(s.status) from waxserver.status s where waid = (select max(waid) from waxserver.werkauftrag w where p.nr = w.preisanfrageNr) ) as Stat,
(select bez from waxserver.freigabestatus s where id = (select max(s.status) from waxserver.status s where waid = (select max(waid) from waxserver.werkauftrag w where p.nr = w.preisanfrageNr) ) ) as StatBezeichung


FROM waxserver.preisanfrage p

where p.nr >=  *PaNr*

and (select max(s.status) from waxserver.status s where waid = (select max(waid) from waxserver.werkauftrag w where p.nr = w.preisanfrageNr)) >= 3
and (select max(s.status) from waxserver.status s where waid = (select max(waid) from waxserver.werkauftrag w where p.nr = w.preisanfrageNr)) <= 4


group by p.nr

