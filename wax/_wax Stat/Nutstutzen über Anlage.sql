SELECT e.artikelnr as Artikelnr, e.dn, e.anschlussdn, sum(w.anzahl) as Anzahl

FROM waxserver.werkauftrag w,
     waxserver.status s,
     waxserver.element e
where w.waid=e.waid
and   w.waid=s.waid
and   s.storniert=0
and   s.freigabedatum >= *WaxDateVon*
and   s.freigabedatum <= *WaxDateBis*
and e.makeanlage = 1
and e.id=2
group by e.dn, e.anschlussdn, e.artikelnr
order by e.dn, e.anschlussdn, e.artikelnr