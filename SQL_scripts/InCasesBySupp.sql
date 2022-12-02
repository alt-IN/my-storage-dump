SELECT c.source,
       c.service,
       c.creator,
       c.code,
       c.rec_date,
       c.ENO,
       c.case_id,
       c.declarant,
       c.bd,
       c.case_type,
       c.case_reason,
       c.status
  FROM (
        SELECT 
          (SELECT dcs2.NAME
            FROM DICT_COM dcs2
            WHERE dcs2.ID = xs2.CATEGORY_ID)                                     AS source,
          doo.CODE                                                               AS code,
          (SELECT ds.NAME
            FROM DICT_SER ds
            WHERE ds.ID = dct.SERVICE_ID)                                        AS service,
		      (SELECT dcs.NAME
            FROM DICT_COM dcs
            WHERE dcs.ID = xs.CATEGORY_ID)                                       AS creator,
          to_char(cc.RECEPTION_DATE,'dd.mm.yyyy')                                AS rec_date,
          (SELECT xc.EXT_CASE_NUMBER
            FROM EXS_C_CASE xc
            WHERE xc.LOCAL_ID = cc.ID)                                           AS ENO,
          cc.DEP_ID || '-' || cc.ID                                       AS case_id,
          pd.LAST_NAME                                                           AS declarant,
          CASE pd.birth_date_complete
            WHEN 1 THEN '00.00.' || TO_CHAR(pd.birth_date, 'yyyy')
            WHEN 2 THEN '00.' || TO_CHAR(pd.birth_date, 'mm.yyyy')
            WHEN 3 THEN TO_CHAR(pd.birth_date, 'dd.mm.yyyy')
            ELSE NULL END                                                        AS bd,
          dct.NAME                                                               AS case_type,
          (SELECT dcr.name
            FROM DICT_CREASON dcr
            WHERE dcr.ID = cc.REASON_ID)                                         AS case_reason,
          dpss.NAME                                                              AS status,
          row_number() OVER (PARTITION BY cc.ID ORDER BY pss.DATE_CREATED DESC ) AS status_num
          --decode(cc.FROM_DRAFT, 0, '�������', 1, '����������')                   AS method,
            FROM C_CASE cc,
                E_SUPP xs,
                DICT_CTYPE dct,
                DEPARTMENT d,
                DICT_OFF_ORG doo,
                C_CASE_PERS_E cpe,
                C_PERSD pd,
                C_PERS_CASE cpc,
                C_PERS_SSTAT pss,
                C_OPER co,
                DICT_PERS_SSTAT dpss,
                DICT_OPER do,
                E_SUPP xs2
          WHERE cc.CREATE_DATE >= to_date('01.02.2021','dd.mm.yyyy')
            AND cc.TYPE_ID IN (20,27,30,35,350,351,40,41,49,46,44,407,408,412,50,51,53,54)
            AND cc.CREATE_SUPP_ID = xs.ID(+)
            AND cc.RECEPT_SUPP_ID = xs2.ID(+)
            AND cc.TYPE_ID = dct.ID
            AND cc.DEP_ID = d.ID
            AND xs.CATEGORY_ID IN (13802,13801)
            AND d.OFF_ORG_ID = doo.ID
            AND doo.ID IN (142512,142513,142554,142588,142519,142589,142580,142510)
            AND cc.ID = cpe.CASE_ID
            AND cpe.CASE_ROLE_ID = 1
            AND cpe.PERSDATA_ID = pd.ID
            AND cc.ID = cpc.ID
            AND cpc.PERS_STATE_ID = pss.PERS_STATE_ID
            AND pss.STATUS_ID = dpss.ID
            AND pss.OPERATION_ID = co.ID
            AND co.TYPE_ID = do.ID) c
 WHERE c.status_num = 1

UNION

SELECT 
    'SUPPLIER'															            AS source,
		(SELECT ds.NAME
      FROM DICT_SER ds
      WHERE ds.ID = dct.SERVICE_ID)                     AS service,
		'SUPPLIER'														              AS creator,
		doo.CODE 														            	  AS code,
		to_char(xdc.CREATE_DATE,'dd.mm.yyyy')               AS rec_date,
		xdc.CASE_NUMBER 												          	AS ENO,
		' '																	              	AS case_id,
		substr(xdc.DECLARANT_NAME, 1, instr(xdc.DECLARANT_NAME, ' ')) 	AS declarant,
		CASE WHEN xdc.TYPE_ID IN (350, 351) THEN EXTRACTVALUE(XMLTYPE(xdc.DATA, NLS_CHARSET_ID('CHAR_CS')),'//out4:applicant/out3:identity/out:birthDate/node()', 'xmlns:out=""http://organization.com/replication/core"" xmlns:out4=""http://organization.com/application/firstDocument"" xmlns:out3=""http://organization.com/replication/core""')
		  WHEN xdc.TYPE_ID IN (27) THEN EXTRACTVALUE(XMLTYPE(xdc.DATA, NLS_CHARSET_ID('CHAR_CS')),'//out4:applicant/out3:identity/out:birthDate/node()', 'xmlns:out=""http://organization.com/replication/core"" xmlns:out4=""http://organization.com/application/secondDocument"" xmlns:out3=""http://organization.com/replication/core""')
		  WHEN xdc.TYPE_ID IN (407, 408, 412) THEN EXTRACTVALUE(XMLTYPE(xdc.DATA, NLS_CHARSET_ID('CHAR_CS')),'//out:declarant/out2:identity/out:birthDate/node()', 'xmlns:out=""http://organization.com/replication/address"" xmlns:out2=""http://organization.com/replication/core""')
		  ELSE NULL END                                      AS bd,
		dct.name 															            	 AS case_type,
		CASE WHEN xdc.TYPE_ID IN (350, 351) THEN (SELECT dcr.name FROM DICT_CREASON dcr 	WHERE dcr.ID = EXTRACTVALUE(XMLTYPE(xdc.DATA, NLS_CHARSET_ID('CHAR_CS')),'//out2:application/out4:applicationReason/out3:element/node()', 'xmlns:out2=""http://organization.com/application/secondDocument/issue"" xmlns:out4=""http://organization.com/application/secondDocument"" xmlns:out3=""http://organization.com/replication/core""'))
		  WHEN xdc.TYPE_ID IN (27) THEN (SELECT dcr.name FROM DICT_CREASON dcr	WHERE dcr.ID = EXTRACTVALUE(XMLTYPE(xdc.DATA, NLS_CHARSET_ID('CHAR_CS')),'//out2:application/out4:applicationReason/out3:element/node()', 'xmlns:out2=""http://organization.com/application/firstDocument/issue"" xmlns:out4=""http://organization.com/application/firstDocument"" xmlns:out3=""http://organization.com/replication/core""'))
		  ELSE NULL END AS case_reason,
		'DRAFT'															          	     AS status
  FROM E_DCASE xdc,
       DICT_CTYPE dct,
       DEPARTMENT d,
       DICT_OFF_ORG doo,
       E_SUPP xs
  WHERE xdc.CREATE_DATE >= to_date('01.04.2021','dd.mm.yyyy')
    AND xdc.CASE_TYPE_ID = dct.ID
    AND xdc.DEPARTMENT_ID = d.ID
    AND xdc.E_SYSTEM_ID = xs.ID
    AND xs.CATEGORY_ID = 13802
    AND d.OFF_ORG_ID = doo.ID
    AND NOT EXISTS(SELECT 1
                      FROM E_C_CASE xc
                    WHERE xc.E_SYSTEM_ID = xdc.E_SYSTEM_ID
                      AND xc.E_DATA_ID = xdc.E_DATA_ID)
