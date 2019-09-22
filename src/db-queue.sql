
SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";

CREATE TABLE `queue` (
	`id` int(11) NOT NULL COMMENT 'Queue entry ID',
	`title` varchar(180) COLLATE utf8mb4_unicode_ci NOT NULL COMMENT 'Requested song title (as seen in tlpr-apis songs table)',
	`requester` int(11) NOT NULL COMMENT 'Requesting user ID',
	`song_id` int(11) NOT NULL COMMENT 'Requested song ID'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

ALTER TABLE `queue` ADD PRIMARY KEY (`id`);
ALTER TABLE `queue` MODIFY `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Queue entry ID', AUTO_INCREMENT=2;

COMMIT;

