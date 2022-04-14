function Out =  OnSegment(p1, p2, p3)
	if(min(p1(1), p2(1)) <= p3(1) <= max(p1(1), p2(1)) && min(p1(2), p2(2)) <= p3(2) <= max(p1(2), p2(2)))
			Out = 1;
	else 
			Out = 0;
	end
end

function Out = SegmentsIntersect(p1, p2, p3, p4)
	d1 = (p1(1) - p3(1)) * (p4(2) - p3(2)) - (p4(1) - p3(1)) * (p1(2) - p3(2));
	d2 = (p2(1) - p3(1)) * (p4(2) - p3(2)) - (p4(1) - p3(1)) * (p2(2) - p3(2));
	d3 = (p3(1) - p1(1)) * (p2(2) - p1(2)) - (p2(1) - p1(1)) * (p3(2) - p1(2));
	d4 = (p4(1) - p1(1)) * (p2(2) - p1(2)) - (p2(1) - p1(1)) * (p4(2) - p1(2));
	if(((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
			Out = 1;
	elseif (d1 == 0 && OnSegment(p3, p4, p1)) 
			Out = 1;
	elseif (d2 == 0 && OnSegment(p3, p4, p2)) 
			Out = 1;
	elseif (d3 == 0 && OnSegment(p1, p2, p3)) 
			Out = 1;
	elseif (d4 == 0 && OnSegment(p1, p2, p4)) 
			Out = 1;
	else 
			Out = 0;
	end
end