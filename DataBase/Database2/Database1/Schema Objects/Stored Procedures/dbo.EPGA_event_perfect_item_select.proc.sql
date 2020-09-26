﻿CREATE PROCEDURE [dbo].[EPGA_event_perfect_item_select]
	@iEventUID [int]
AS
set nocount on  
set transaction isolation level read uncommitted  
/*
    20090305. mk8253.
    이벤트 장착 아이템 리스트
    
    { call dbo.EPGA_event_perfect_item_select ( %d ) }
    { call dbo.EPGA_event_perfect_item_select ( @1 ) }
    @1 ; iEventUID int
    
    n return ( @1 )
    @1 ; Type int
    @2 ; ItemID int
    @3 ; Chartype int
*/

select
    Type
,   ItemID
,   CharType
from dbo.EPGAEventPerfectItem as a
join dbo.GoodsInfoList as b
on a.ItemID = b.GoodsID
where
    EventUID = @iEventUID

