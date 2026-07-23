SELECT wanr,	werkauftrag.waid,	classid, sdauftragsnr, sdpositionsnr, sdmaterialnr, kcauftragsnr, kcposnr, pspelement	,posnr FROM status LEFT JOIN  werkauftrag ON ( status.waid = werkauftrag.waid )
WHERE wanr in
()