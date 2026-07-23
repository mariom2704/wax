SELECT 'Scheibe', v.eleid, st.muffenID, st.muffenDN as Muffen_DN, r.dnvl as Rohrdurchmesser,
       sign(st.muffenrelabst1) + sign(st.muffenrelabst2) + sign(st.muffenrelabst3) + sign(st.muffenrelabst4) +
       sign(st.muffenrelabst5) + sign(st.muffenrelabst6) + sign(st.muffenrelabst7) + sign(st.muffenrelabst8) +
       sign(st.muffenrelabst9) + sign(st.muffenrelabst10) as Anz_Muffen_pro_Rohr, sum(w.anzahl) AS AnzahlScheiben
	b.Deutsch as Bezeichnung, r.laenge as Rohrlaenge
FROM waxserver.werkauftrag w,
     waxserver.strangrohr st,
     waxserver.status s,
     waxserver.rohrleitung r,
     waxstamm.bauelementgruppen b,
     waxserver.vk3listewa v
where w.waid=st.waid
and   w.waid=s.waid
and   w.waid=r.waid
and   st.muffenID=b.elemid
and   w.waid=v.waid
and   s.storniert=0
and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*
and   (v.eleid = 15)
group by st.muffenID, Anz_Muffen_pro_Rohr, Rohrdurchmesser, Muffen_DN
order by st.muffenID, Rohrlaenge