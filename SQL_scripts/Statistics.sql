-------- File upload audit for last 5 days--------------------------------------------------------------------------------------------------------------------------

WITH a1 (num,evtype,fname,fid,can,cdate) AS
(   SELECT 
        count(*) num,
        event_type,
        id.filename, 
        idpl.file_id, 
        sd.canceled, 
        id.created_date::date 
    FROM inbound_data_process_log idpl 
        JOIN inbound_data id ON idpl.file_id = id.id  
        LEFT JOIN storage_data sd ON idpl.external_id = sd.external_id AND idpl.file_id = sd.file_id
    WHERE event_type IN ('RECORD_STORED', 'RECORD_DUPLICATE','RECORD_UPDATED', 'RECORD_PARSED')
        --AND id.created_date::date >= TO_DATE(:d1,'dd.mm.yyyy')
        --AND id.created_date::date <= TO_DATE(:d2,'dd.mm.yyyy')
        AND id.created_date::date >= CURRENT_DATE - INTERVAL '5 DAYS'
        AND id.created_date::date <= CURRENT_DATE
    GROUP BY event_type, id.filename, idpl.file_id, sd.canceled, id.created_date ),
a2 (fid,extnum,cdate) AS 
(   SELECT 
        idpl2.file_id, 
        count(idpl2.external_id), 
        idpl2.created_date::date 
    FROM inbound_data_process_log idpl2 
    JOIN (SELECT 
        max (idpl.id) id, 
        idpl.event_type , 
        idpl.external_id 
        FROM storage_data sd2 
            JOIN inbound_data_process_log idpl on sd2.external_id = idpl.external_id 
        WHERE canceled = true
            AND event_type in ('RECORD_UPDATED')--'RECORD_STORED'
            --AND date_trunc('second', sd2.created_date::timestamp) < date_trunc('second', idpl.event_date::timestamp)
        GROUP BY 2,3) idpl3 ON idpl3.id = idpl2.id
    --WHERE idpl2.created_date::date >=  TO_DATE(:d1,'dd.mm.yyyy')
    --AND idpl2.created_date::date <= TO_DATE(:d2,'dd.mm.yyyy')
    WHERE idpl2.created_date::date >= CURRENT_DATE - INTERVAL '5 DAYS'
        AND idpl2.created_date::date <= CURRENT_DATE
    GROUP BY 1,3)

SELECT 
    a1.fid "FILE NUM", 
    a1.fname "FILE NAME", 
    a1.cdate "UPLOAD DATE",
    SUM(CASE a1.evtype WHEN 'RECORD_PARSED' THEN a1.num ELSE 0 END) AS "NUMBER OF RECORDS",
    SUM(CASE WHEN a1.evtype IN ('RECORD_PARSED') THEN a1.num ELSE 0 END) - SUM(CASE WHEN a1.evtype IN ('RECORD_STORED','RECORD_UPDATED', 'RECORD_DUPLICATE') THEN a1.num ELSE 0 END) AS "WITH ERROR",
    SUM(CASE a1.evtype WHEN 'RECORD_STORED' THEN a1.num ELSE 0 END) AS "CREATED",
    SUM(CASE a1.evtype WHEN 'RECORD_DUPLICATE' THEN a1.num ELSE 0 END) AS "DUPLICATES",
    SUM(CASE a1.evtype WHEN 'RECORD_UPDATED' THEN a1.num ELSE 0 END) AS "UPDATED",
    --sum(CASE when a1.evtype IN ('RECORD_STORED','RECORD_UPDATED')  AND a1.can = true then a1.num else 0 end) AS "CANCEL FLAG",
    MAX(CASE WHEN a1.fid=a2.fid THEN a2.extnum ELSE 0 END) AS "CANCELED",
    MAX(CASE WHEN a1.fid=a3.fid THEN a3.num ELSE 0 END) AS "CREATED WITH CANCEL"
    FROM a1
        LEFT JOIN a2 ON a1.fid=a2.fid
        LEFT JOIN (SELECT 
                    count(id), 
                    file_id 
                FROM storage_data sd 
                WHERE sd.canceled = true
                    AND NOT EXISTS (SELECT 1 
                        FROM inbound_data_process_log idpl4 
                        WHERE sd.external_id = idpl4.external_id 
                            AND idpl4.event_type = 'RECORD_UPDATED')
                GROUP BY 2) a3 (num,fid) ON a1.fid=a3.fid
    WHERE a1.fname NOT LIKE 'rabbitmq.exchange'
    GROUP BY a1.fid, a1.fname, a1.cdate
    ORDER BY 1 DESC;



-------- All users audit by date period--------------------------------------------------------------------------------------------------------------------------

SELECT  
        context ->> 'source' AS src,
        request -> 'request-attributes' ->> 'instance-id' AS region,
        request -> 'request-attributes' ->> 'department-name' AS department,
        request -> 'request-attributes' ->> 'user-name' AS request_user,
        request ->> 'record-id' AS id,
        request ->> 'full-name' AS search_person,
        request -> 'request-attributes' ->>'case-service-name' AS service_type,
        request -> 'request-attributes' ->> 'case-type-name' AS case_type,
        created_date 
    FROM requests_audit ra 
    WHERE created_date::date >= TO_DATE('13.06.2021','dd.mm.yyyy')
        AND created_date::date < TO_DATE('23.06.2021','dd.mm.yyyy');



-------- First and Last name data by file upload --------------------------------------------------------------------------------------------------------------------------

SELECT 
        idpl.file_id "FILE NUMBER", id.filename "FILE NAME",
        CASE  WHEN idpl.event_type = 'RECORD_STORED' THEN 'CREATED'
            WHEN idpl.event_type = 'RECORD_DUPLICATE' THEN 'DUPLICATED'
            WHEN idpl.event_type = 'RECORD_UPDATED' THEN 'UPDATED' 
            ELSE NULL END "EVENT TYPE",
        idpl.external_id "RECORD ID ", 
        sd.data_raw ->> 'full-name' AS "FIRST&LAST NAME",
        CASE WHEN sd.canceled = true THEN 'CANCELED'
            WHEN sd.canceled = false THEN 'ACTIVE' 
            ELSE NULL END "CANCEL FLAG"
    FROM inbound_data_process_log idpl 
        JOIN inbound_data id ON idpl.file_id = id.id 
        JOIN storage_data sd ON idpl.external_id = sd.external_id 
    WHERE idpl.file_id = 11
        AND idpl.event_type IN ('RECORD_STORED', 'RECORD_DUPLICATE','RECORD_UPDATED');
