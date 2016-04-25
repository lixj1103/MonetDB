-- This Source Code Form is subject to the terms of the Mozilla Public
-- License, v. 2.0.  If a copy of the MPL was not distributed with this
-- file, You can obtain one at http://mozilla.org/MPL/2.0/.
--
-- Copyright 1997 - July 2008 CWI, August 2008 - 2016 MonetDB B.V.

-------------------------------------------------------------------------
----------------------- GRID related functions --------------------------
-------------------------------------------------------------------------
CREATE FILTER FUNCTION distance(x1 HUGEINT,  y1 HUGEINT,  x2 HUGEINT,  y2 HUGEINT,  distance double) EXTERNAL NAME grid.distance;