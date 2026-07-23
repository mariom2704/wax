SELECT s.wanr, wa.waid,  eleid,  wa.anzahllaenge as Anzahl, wa.dn, wa.anschlussdn FROM waxserver.vk3listewa wa,  waxserver.verteiler st, waxserver.status s 
WHERE wa.waid = st.waid
and s.waid = st.waid
and s.storniert=0
and s.freigabedatum >= *WaxDateVon*
and s.freigabedatum <= *WaxDateBis*
and ( wa.anschlussdn = 65 or wa.anschlussdn = 80)
and (eleid = 5 or eleid = 39) and (dn = 20 or dn = 25);