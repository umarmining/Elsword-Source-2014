﻿ALTER TABLE [dbo].[CSGACoupleSystemEquip] ADD
CONSTRAINT [FK_CSGACoupleSystemEquip_CSGACoupleSystem] FOREIGN KEY ([LoginUID]) REFERENCES [dbo].[CSGACoupleSystem] ([LoginUID]) ON DELETE CASCADE ON UPDATE CASCADE

