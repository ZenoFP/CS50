-- Keep a log of any SQL queries you execute as you solve the mystery.

-- See the schema of fiftyville.db by typing sqlite3 fiftyville.db and then .schema
.schema

-- see crime scene report on the case of fiftyville as suggested in the problem set
SELECT description FROM crime_scene_reports WHERE street = 'Chamberlin Street' AND year = '2020' AND day = '28' AND month = '7';
theft took place at 10:15 am of 28/07/2020

-- as output, three out of three interviews with witnesses are related to the couthouse, see the interviews in TABLE interviews
SELECT transcript, name FROM interviews WHERE year = '2020' AND day = '28' AND month = '7';

1)  Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away.
    If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame. | Ruth

2)   I don't know the thief's name, but it was someone I recognized.
     Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money. | Eugene

3)   As the thief was leaving the courthouse, they called someone who talked to them for less than a minute.
     In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. | Raymond

-- 1) control which cars have left the parking lot of the courthouse in that timeframe
SELECT activity, license_plate, hour, minute  FROM courthouse_security_logs WHERE year = '2020' AND month = '7' AND day = '28' ORDER BY activity, hour ;

--outputs that are interesting:
exit | 5P2BI95 | 10 | 16
exit | 94KL13X | 10 | 18
exit | 6P58WS2 | 10 | 18
exit | 4328GD8 | 10 | 19
exit | G412CB7 | 10 | 20
exit | L93JTIZ | 10 | 21
exit | 322W7JE | 10 | 23
exit | 0NTHK55 | 10 | 23
exit | 1106N58 | 10 | 35


-- 2) control money withdrawal from atm on fifer street
SELECT account_number, transaction_type FROM atm_transactions WHERE atm_location = 'Fifer Street' AND year = '2020' AND month = '7' AND day = '28';
28500762 | withdraw
28296815 | withdraw
76054385 | withdraw
49610011 | withdraw
16153065 | withdraw
86363979 | deposit
25506511 | withdraw
81061156 | withdraw
26013199 | withdraw

-- 3) control calls made that day that lasts less than one minute
SELECT caller, receiver FROM phone_calls WHERE year = '2020' AND month = '7' AND day = '28' AND duration < 60;
(130) 555-0289 | (996) 555-8899
(499) 555-9472 | (892) 555-8872
(367) 555-5533 | (375) 555-8161
(499) 555-9472 | (717) 555-1342
(286) 555-6063 | (676) 555-6554
(770) 555-1861 | (725) 555-3243
(031) 555-6622 | (910) 555-3251
(826) 555-1652 | (066) 555-9701
(338) 555-6650 | (704) 555-2131


-- 1 + 3) starting now by controlling name of peoples with the license plates founded before and the telephone numbers found
SELECT name FROM people WHERE license_plate IN (SELECT license_plate  FROM courthouse_security_logs WHERE year = '2020' AND month = '7' AND day = '28' AND hour = '10' AND activity = 'exit' AND minute > 15 AND minute < 36)
AND phone_number IN ( SELECT caller FROM phone_calls WHERE year = '2020' AND month = '7' AND day = '28' AND duration < '60');
name

Roger
Madison
Russell
Evelyn
Ernest
--i also controlled receiver license plates because maybe the car used for the theft was of the receiver of the call, NO OUTPUTS GET.


--2) Control on bank accounts who made transaction in that street ATM that day by controlling the previously given bank accounts
SELECT name FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE atm_location = 'Fifer Street' AND transaction_type = 'withdraw' AND year = '2020' AND month = '7' AND day = '28'));

name

Bobby
Elizabeth
Victoria
Madison
Roy
Danielle
Russell
Ernest
-- OK until there


--search for collisions: find this information combining the last 2 queries (1+2+3)
SELECT name FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE atm_location = 'Fifer Street' AND transaction_type = 'withdraw' AND year = '2020' AND month = '7' AND day = '28'))
AND name IN (SELECT name FROM people WHERE license_plate IN (SELECT license_plate  FROM courthouse_security_logs WHERE year = '2020' AND month = '7' AND day = '28' AND hour = '10' AND activity = 'exit' AND minute > 15 AND minute < 36)
AND phone_number IN ( SELECT caller FROM phone_calls WHERE year = '2020' AND month = '7' AND day = '28' AND duration < '60'));
--from the outputs you can see Russell, Madison, Ernest. These are the suspected peoples. (exit from parking after 10.15, made call last < than 1 minute and made atm withdrawal)

--control if they called someone that day and who.
SELECT name FROM people WHERE phone_number IN(
SELECT receiver
FROM phone_calls
WHERE year = '2020'
AND month = '7'
AND day = '28'
AND duration < '60'
AND caller IN (SELECT phone_number FROM people WHERE name = 'Russell' OR name = 'Madison' OR name = 'Ernest'));
--Suspected for accomplice are James Philip and Berthold, but i will go further without knowing who they are helping because i will search for this names in the flights


--control who took flights from fiftyville the day after
SELECT name
FROM people
WHERE passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id IN
(SELECT id FROM flights WHERE day = 29 AND year = 2020 AND month = 7 AND origin_airport_id IN
(SELECT id FROM airports WHERE city = 'Fiftyville')))
AND phone_number IN (
SELECT caller
FROM phone_calls
WHERE year = '2020'
AND month = '7'
AND day = '28'
AND duration < '60'
AND caller IN (SELECT phone_number FROM people WHERE name = 'Russell' OR name = 'Madison' OR name = 'Ernest'));
--Madison, Russell and Ernest took a flight from Fiftyville to other cities that day;

--Sure if one of them is the thief, he or she never came back to fiftyville for sure! Let's Check
SELECT city
FROM airports
WHERE id
IN (SELECT destination_airport_id FROM flights WHERE year = 2020)
AND id IN (SELECT destination_airport_id FROM flights WHERE id IN (SELECT flight_id FROM passengers WHERE passport_number IN (SELECT passport_number FROM people WHERE name = 'Ernest') ) );
--Done the same with Madison Russell and Ernest and Result: Madison and Ernest are the only ones who only took one flight on the whole 2020 year, the same flight directed to London.

--print name of the people on that flight to london to discover what accomplice was there
SELECT name
FROM people
WHERE passport_number
IN(SELECT passport_number FROM passengers WHERE flight_id
IN(SELECT id FROM flights WHERE destination_airport_id
IN (SELECT id FROM airports WHERE city = 'London' ) AND day = 29 AND year = 2020 AND month = 7 ) );
-- NO ONE OF THE SUSPECTS FOUND

--maybe the Accomplice is already at London because he is from London, and then they're excaping to another city?
SELECT city
FROM airports
WHERE id
IN (SELECT destination_airport_id FROM flights WHERE year = 2020 AND id
IN(SELECT flight_id FROM passengers WHERE passport_number
IN (SELECT passport_number FROM people WHERE name = 'James')) );
--both James and Philip flew to both London and Fiftyville but Berthold never did a flight in all the 2020.
--So it was simply waiting for the thief!

--Berthold is the Accomplice and London is the city they escaped to, now who called Berthold that day (that is one of the suspects also)?
SELECT people.name
FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE phone_calls.receiver IN (SELECT phone_number FROM people WHERE name = 'Berthold')
AND phone_calls.receiver IN (SELECT receiver FROM phone_calls WHERE year = '2020' AND month = '7' AND day = '28' AND duration < 60)
AND phone_calls.caller IN (SELECT caller FROM phone_calls WHERE year = '2020' AND month = '7' AND day = '28' AND duration < 60);

-- Outputs only Ernest! One of the suspects!
--Answers for me are so: Ernest, London, Berthold.