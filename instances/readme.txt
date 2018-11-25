EXTENDED SINGLE VEHICLE TRUCK AND TRAILER ROUTING PROBLEM 

The file format derives from a mix of other file formats from which the problem is inspired.

INSTANCES FILE DESCRIPTION

. n1 = number of satellites
. n2 = number of truck customers
. n3 = number of vehicle customers without parking facilities
. n4 = number of vehicle customers with parking facilities
. truck capacity
. trailer capacity
. depot definition as: vertex-id    x-coordinate    y-coordinate
. the following n1 lines contain the satellites definition as: vertex-id     x-coordinate    y-coordinate
. the following n2 lines contain the truck customers definition as: vertex-id   x-coordinate    y-coordinate    customer-request
. the following n3 lines contain the vehicle customers (without parking facilities) definition as: vertex-id x-coordinate    y-coordinate    customer-request
. the following n4 lines contains the vehicle customers (with parking facilities) definition as: vertex-id x-coordinate    y-coordinate    customer-request

The previously described file format applies to instances in the directories:
. chao
. xsttrp
. sttrpsd
. bartolini

SINGLE TRUCK AND TRAILER ROUTING PROBLEM WITH SATELLITE DEPOTS (STTRPSD)
Refer to the original readme inside the STTRPSD directory.

LOCATION ROUTING PROBLEM (LRP)
Refer to the original readme inside the lrp_tuzun directory.
