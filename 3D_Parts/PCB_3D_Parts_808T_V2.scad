$fn = 100;

//Main parameters to tune : Configured for MatterControl
$Rail6mmDiameter = 6.2;//Hole for X 6mm rail attachments to Y carriages and Y rails to base supports - Good
$LeadScrewAttachHoleDiameter = 16.3;//Leadscrew block hole for T-threads
$VerticalLockClearance = .4;//Delta between vertical threaded hole and free hole
$HorizontalLockClearance = .35;//Delta between horizontal threaded hole and free hole
$LockScrewBoreM2_5Vertical = 2.4;//3D printed vertical hole for M2.5 threadlock - Good
$LockScrewBoreM2_5Horizontal = 2.3;//3D printed horizontal hole for M2.5 threadlock
$LockScrewBoreM3Vertical = 3.0;
$LockScrewBoreM3_5Vertical = 3.4;
$LockScrewBoreM3Horizontal = 2.85;
$LockScrewBoreM8Vertical = 7.8;//Vertical M8 threadlocked hole - Good
$Shaft3mmHorizontal = 3.05;
$Bearing8mmDiameter = 22.2;
$LockScrewBoreM2_5Laser = 2.2;//Laser mounting holes for leadscrew block and limit switches
$LockScrewBoreM3Laser = 2.7;
$LockScrewBoreM2Laser = 1.7;//Laser hole size for M2 thread locking
$Bearing6mmDiameter = 12.2;
$XMotorMountSpacing = 11.5;
$BearingShaftDiameter = 8.1;
$XMotorMountExtension = 4; // Extend the X motor Y plate mounting hole lengths to accomodate different motors

$HeightAdjust = 19;//Allow adjustment for overall carriage height for 808T focus requierments. 19 is good since it aligns with original y plate holes to allow dual fitting
$DoTall = 0;

$PlateClearance = .15;

$AcrylicThickness = 3.0;
$BaseAcrylicThickness = 10;
$AlignmentAcrylicThickness = 1.5;


$BaseAcrylicColor = [0.1, 0.5, 0.0, 0.9];
$FrameAcrylicColor = [0.7, 0.8, 0.2, 0.9];
$AlignmentAcrylicColor = [0.3, 0.7, 0.7, 0.4];
$BaseAcrylicColor = [0.1, 0.1, 0.5, 0.6];

$YPosition = 130;
$XPosition = 150;

$RoundToHex = 1.155;

$XRailSpacing = 300;
$CarrierSpacing = 40;
$CarrierZOffset = 14;
$LaserCarrierWidth = 58;
$LaserCarrierLength = 68;
$XRailLength = 300;
$YRailLength = 270;

module RodClamp()
{
  //width = width of main block
  //height = height of main block
  //depth = depth of main block
  //boreheight = height of center of shaft from base
  //boreradius = radius of bore
  //gapwidth = width of clamp gap
  //screwbore = bore hole radius for clamp screw
  //screwstyle = 0-hex side aligned, 1-hex top aligned, 2-round
  //captivestyle = 0-hex side aligned, 1-hex top aligned, 2-square open to top, 3-hex base open top
  //captivebore = bore hole radius of captive nut
  //captivedepth = thicknes of captuve nut
  //screwoffset = height of clamp screw hole delta from centered

  upper = (height - boreheight - boreradius);

//    screwoffset = (upper / 2) + boreradius;
    screwheight = (upper / 2) + boreradius + screwoffset;

  difference()
  {
    translate([-width/2, -depth/2, -boreheight])
      cube([width, depth, height]);
    rotate(90, [1, 0, 0])
      cylinder(r = boreradius, h = depth + 0.1, center = true);
    translate([0, -(depth + 0.1) / 2, 0])
      rotate(-45, [0, 1, 0])
      cube([boreradius, depth + 0.1, boreradius]);
    translate([-gapwidth / 2, -(depth + 0.1) / 2, 0])
    cube([gapwidth, depth + 0.1, height - boreheight + 0.1]);

    translate([0, 0, screwheight])
    {
      rotate(90, [0, 1, 0])
        if (screwstyle == 0)
          cylinder(r = screwbore, h = width + 0.1, center = true, $fn = 6);
        else if (screwstyle == 1)
          rotate(30, [0, 0, 1])
          cylinder(r = screwbore / 0.866025, h = width + 0.1, center = true, $fn = 6);
        else if (screwstyle == 2)
          cylinder(r = screwbore / 0.866025, h = width + 0.1, center = true);
      translate([((width - captivedepth) / 2) + 0.1, 0, 0])
      {
      if (captivestyle == 0)
        rotate(90, [0, 1, 0])
          cylinder(r = captivebore / 0.866025, h = captivedepth + 0.1, center = true, $fn = 6);
      else if (captivestyle == 1)
        rotate(90, [0, 1, 0])
          rotate(30, [0, 0, 1])
            cylinder(r = captivebore / 0.866025, h = captivedepth + 0.1, center = true, $fn = 6);
      else if (captivestyle == 2)
        translate([0, 0, ((upper + (captivebore * 2)) / 4) - ((captivebore * 2)/ 1.8)])
        cube([captivedepth + 0.1, captivebore * 2, ((upper + (captivebore * 2)) / 1.8)], center = true);
      else if (captivestyle == 3)
      {
        translate([0, 0, ((upper + captivebore) / 4)])
        cube([captivedepth + 0.1, captivebore * 2, ((upper + captivebore) / 1.7)], center = true);
        rotate(90, [0, 1, 0])
          cylinder(r = captivebore / 0.866025, h = captivedepth + 0.1, center = true, $fn = 6);
      }
      }
    }
  }
}

module LaserCarrier()
{
  translate([-31, -38.5, -7])
  difference()
  {
    //Base mount
    cube([$LaserCarrierWidth, $LaserCarrierLength, 21]);
    //Mechanism opening
    translate([4, 4, -0.1])
      cube([$LaserCarrierWidth - 8, 68 - 8, 21.2]);
//    //FPC opening, horizontal
//    translate([-1, 23.5, -0.1])
//      cube([20, 30, 5]);
    //Rail support 1
    translate([-0.1, 11, 3])
      rotate(90, [0, 1, 0])
        rotate(30, [0, 0, 1])
          cylinder(d = $Shaft3mmHorizontal * $RoundToHex, h = 70, $fn = 6);
    //Rail support 2
    translate([-0.1, 11 + 48, 3])
      rotate(90, [0, 1, 0])
        rotate(30, [0, 0, 1])
          cylinder(d = $Shaft3mmHorizontal * $RoundToHex, h = 70, $fn = 6);
  }
}

module SideRailClamp()
{
  difference()
  {
	translate([0, 0, ($HeightAdjust * $DoTall)])
    rotate(90, [0, 0, 1])
      RodClamp(width = 15, height = 21 + ($HeightAdjust * $DoTall), screwoffset = 0, depth = 18, boreheight = 6 + ($HeightAdjust * $DoTall), boreradius = ($Rail6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 0);
	translate([0, 0, ($HeightAdjust * $DoTall)])
	{
    //Rail clamp hole
    translate([0, 10, 0])
      rotate(90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 20);
//    //Thread locked section
//    cylinder(d = $LockScrewBoreM2_5Vertical, h = 25);
//    //Screw head countersink section
//    translate([0, 0, 7])
//      cylinder(d = 6.3, h = 25);
    //Top plate mounting holes
    translate([0, 5, -5])
      cylinder(d = $LockScrewBoreM2_5Vertical, h = 25);
    translate([0, -5, -5])
      cylinder(d = $LockScrewBoreM2_5Vertical, h = 25);
	}
  }
}

module SideRailCarriage()
{
  //Lower side rail rider for bearing
  difference()
  {
    RodClamp(width = 18, height = 18, screwoffset = 0, depth = 55, boreheight = 10, boreradius = ($Bearing6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 1.5);
    //Lock screw hole
    translate([-15, 0, 0])
      rotate(90, [0, 1, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 30);
  }
  translate([0, -$CarrierSpacing / 2, $CarrierZOffset])
    SideRailClamp();
  difference()
  {
    translate([0, $CarrierSpacing / 2, $CarrierZOffset])
      SideRailClamp();
    translate([0, 27.5, 28 + $HeightAdjust])
      rotate(90, [0, 1, 0])
        rotate(-90, [1, 0, 0])
          LimitSwitchHoles();
  }
}

module BeltClampHolePairVertical()
{
  translate([0, 0, -7])
    rotate(-90, [0, 1, 0])
      cylinder(d = $Bore, h = 20);
  translate([0, 0, 8])
    rotate(-90, [0, 1, 0])
      cylinder(d = $Bore, h = 20);
}

module XCarriageFixedDiode()
{
  difference()
  {
    union()
    {
      translate([-7, $CarrierSpacing / 2, $CarrierZOffset])
        rotate(90, [0, 0, 1])
          difference()
          {
            RodClamp(width = 18, height = 25, screwoffset = 0, depth = 50, boreheight = 14, boreradius = ($Bearing6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 1.5);
            //Belt clamp holes
            translate([-1.8, 21, 0])
              BeltClampHolePairVertical($Bore = $LockScrewBoreM2_5Horizontal);
            translate([-1.8, -21, 0])
              BeltClampHolePairVertical($Bore = $LockScrewBoreM2_5Horizontal);
           }
      translate([-7, -$CarrierSpacing / 2, $CarrierZOffset - 0])
      {
        rotate(90, [0, 0, 1])
          RodClamp(width = 18, height = 23, screwoffset = 0, depth = 40, boreheight = 14, boreradius = ($Bearing6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 1.5);
      }

      //Limit switch bumper
      translate([16, 26.5, 0])
        cube([17, 6, 25]);

      //Diode mounting block and runner support
      translate([-22, -12, 0])
      cube([30, 24, 12]);
    }
    
    //Bearing lock screw holes
    translate([0, 34, 14])
      rotate(90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 10);
    translate([0, -24, 14])
      rotate(90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 10);

	//Diode mounting hole
      translate([-7, 6, -0.01])
	cylinder(d1 = 12.25, d2 = 12.0, h = 30);
        
  }
}



module XCarriage()
{
  difference()
  {
    union()
    {
      translate([-7, $CarrierSpacing / 2, $CarrierZOffset])
        rotate(90, [0, 0, 1])
          difference()
          {
            RodClamp(width = 18, height = 25, screwoffset = 0, depth = $LaserCarrierLength, boreheight = 14, boreradius = ($Bearing6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 1.5);
            //Belt clamp holes
            translate([0, 23, 0])
              BeltClampHolePairVertical($Bore = $LockScrewBoreM2_5Horizontal);
            translate([0, -15, 0])
              BeltClampHolePairVertical($Bore = $LockScrewBoreM2_5Horizontal);
           }
      translate([-7, -$CarrierSpacing / 2, $CarrierZOffset - 0])
      {
        rotate(90, [0, 0, 1])
          RodClamp(width = 18, height = 23, screwoffset = 0, depth = $LaserCarrierLength, boreheight = 14, boreradius = ($Bearing6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 1.5);
      }

      //Laser carrier
      translate([-11.5,2, -10])
        rotate(90, [0, 0, 1])
          LaserCarrier();

      //Limit switch bumper
      translate([23, 26.5, -17])
        cube([4, 6, 42]);
    }
    //FPC opening, vertical
    translate([-($LaserCarrierLength / 2) - 10, -12, -2])
      cube([$LaserCarrierLength + 10, 24, 40]);
    
    //Bearing lock screw holes
    translate([-10, 20, 20])
      cylinder(d = $LockScrewBoreM3Vertical, h = 10, $fn = 30);
    translate([0, -24, 14])
      rotate(90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM3Vertical * $RoundToHex, h = 10, $fn = 6);

    //Belt clamp access holes
    //Lower
//    translate([-22, 8, 7])
//      rotate(95, [1, 0, 0])
//        cylinder(d = 6, h = 55, $fn = 6);
    translate([8, 8, 7])
      rotate(95, [1, 0, 0])
        cylinder(d = 6, h = 55, $fn = 6);
    //Upper
//    translate([-22, 8, 22])
//      rotate(81.5, [1, 0, 0])
//        cylinder(d = 6, h = 55, $fn = 6);

//    translate([8, 8, 22])
//      rotate(81.5, [1, 0, 0])
//        cylinder(d = 6, h = 55, $fn = 6);

    translate([8, -15, 22.1])
      cube([6, 40, 6], center = true);
//      rotate(90, [1, 0, 0])
//#        cylinder(d = 6, h = 55, $fn = 6);
        
    //Clearance for motor pulley
    translate([13, 10, 7.9])
      cube([15, 16.1, 27]);
    //Clearance for belt 
    translate([-48, -32, 3.9])
      cube([30, 26.1, 27]);
  }
}

module YRailClampTall()
{
  translate([0,0, 20])
  difference()
  {
    RodClamp(width = 18, height = 56, screwoffset = 2, depth = 10, boreheight = 46, boreradius = ($Rail6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 1, captivestyle = 1, captivebore =0, captivedepth = 1.5);
    //Spacer/Y drive shaft bearing plate holes
    translate([5, 10, 7])
      rotate(90, [1, 0, 0])
        cylinder(d = ($LockScrewBoreM3Horizontal + $HorizontalLockClearance) * $RoundToHex, h = 20, $fn = 6);
    translate([-5, 10, 7])
      rotate(90, [1, 0, 0])
        cylinder(d = ($LockScrewBoreM3Horizontal + $HorizontalLockClearance) * $RoundToHex, h = 20, $fn = 6);
    translate([5, 10, -12])
      rotate(90, [1, 0, 0])
        cylinder(d = ($LockScrewBoreM3Horizontal + $HorizontalLockClearance) * $RoundToHex, h = 20, $fn = 6);
    translate([-5, 10, -12])
      rotate(90, [1, 0, 0])
        cylinder(d = ($LockScrewBoreM3Horizontal + $HorizontalLockClearance)  * $RoundToHex, h = 20, $fn = 6);
    //Top mounting lock screw hole
    cylinder(d = $LockScrewBoreM2_5Vertical, h = 20);
  }
  //Mounting base
  translate([-(32 / 2), -5, -26])
  {
    difference()
    {
      cube([32, 10, 10]);
      translate([3, 5, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 20);
      translate([32 - 3, 5, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 20);
    }
  }
}

module YRailClamp()
{
  difference()
  {
    union()
    {
      RodClamp(width = 18, height = 37, screwoffset = 2, depth = 10, boreheight = 26, boreradius = ($Rail6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 1, captivestyle = 1, captivebore =0, captivedepth = 1.5);
      translate([0, 0, $HeightAdjust])
        RodClamp(width = 18, height = $HeightAdjust + 1, screwoffset = 2, depth = 10, boreheight = $HeightAdjust - 10, boreradius = ($Rail6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 1, captivestyle = 1, captivebore =0, captivedepth = 1.5);
    }
	translate([0, 0, $HeightAdjust])
	{
	    //Spacer/Y drive shaft bearing plate holes
	    translate([5, 10, 7])
	      rotate(90, [1, 0, 0])
	        cylinder(d = ($LockScrewBoreM3Horizontal + $HorizontalLockClearance) * $RoundToHex, h = 20, $fn = 6);
	    translate([-5, 10, 7])
	      rotate(90, [1, 0, 0])
	        cylinder(d = ($LockScrewBoreM3Horizontal + $HorizontalLockClearance) * $RoundToHex, h = 20, $fn = 6);
	    translate([5, 10, -12])
	      rotate(90, [1, 0, 0])
	        cylinder(d = ($LockScrewBoreM3Horizontal + $HorizontalLockClearance) * $RoundToHex, h = 20, $fn = 6);
	    translate([-5, 10, -12])
	      rotate(90, [1, 0, 0])
	        cylinder(d = ($LockScrewBoreM3Horizontal + $HorizontalLockClearance)  * $RoundToHex, h = 20, $fn = 6);
	    translate([5, 10, -31])
	      rotate(90, [1, 0, 0])
	        cylinder(d = ($LockScrewBoreM3Horizontal + $HorizontalLockClearance) * $RoundToHex, h = 20, $fn = 6);
	    translate([-5, 10, -31])
	      rotate(90, [1, 0, 0])
	        cylinder(d = ($LockScrewBoreM3Horizontal + $HorizontalLockClearance)  * $RoundToHex, h = 20, $fn = 6);
	}
  
  //Lock screw holes
	translate([-20, 0, 0])
    rotate(90, [0, 1, 0])
      cylinder(d = $LockScrewBoreM2_5Vertical, h = 40);
	translate([-20, 0, $HeightAdjust])
    rotate(90, [0, 1, 0])
      cylinder(d = $LockScrewBoreM2_5Vertical, h = 40);
  }
  //Mounting base
  translate([-(32 / 2), -5, -26])
  {
    difference()
    {
      cube([32, 10, 10]);
      translate([3, 5, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 20);
      translate([32 - 3, 5, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 20);
    }
  }
}

/*
module PlateHoleSet()
{
  translate([4, 20, 4])
    rotate(90, [1, 0, 0])
      cylinder(d = $LockScrewBoreM3Laser + $PlateClearance, h = 30);
  translate([14, 20, 4])
    rotate(90, [1, 0, 0])
      cylinder(d = $LockScrewBoreM3Laser + $PlateClearance, h = 30);
  translate([4, 20, 23])
    rotate(90, [1, 0, 0])
      cylinder(d = $LockScrewBoreM3Laser + $PlateClearance, h = 30);
  translate([14, 20, 23])
    rotate(90, [1, 0, 0])
      cylinder(d = $LockScrewBoreM3Laser + $PlateClearance, h = 30);
  translate([9, 20, 16])
    rotate(90, [1, 0, 0])
      cylinder(d = 6.5, h = 30);
}
*/

module PlateHoleSet()
{
  translate([4, 4])
      circle(d = $LockScrewBoreM3Laser + $PlateClearance);
  translate([14, 4])
      circle(d = $LockScrewBoreM3Laser + $PlateClearance);
  translate([4, 23])
      circle(d = $LockScrewBoreM3Laser + $PlateClearance);
  translate([14, 23])
      circle(d = $LockScrewBoreM3Laser + $PlateClearance);
  translate([9, 16])
      circle(d = 6.5, h = 30);
}

/*
module YPlateMotorHoles()
{
  translate([15.5, 0, 15.5])
    rotate(90, [1, 0, 0])
      cylinder(d = $MountBore, h = 15);
  translate([-15.5, 0, 15.5])
    rotate(90, [1, 0, 0])
      cylinder(d = $MountBore, h = 15);
  translate([15.5, 0, -15.5])
    rotate(90, [1, 0, 0])
      cylinder(d = $MountBore, h = 15);
  translate([-15.5, 0, -15.5])
    rotate(90, [1, 0, 0])
      cylinder(d = $MountBore, h = 15);
}
*/

module YPlateMotorHoles2D()
{
  translate([15.5, 15.5])
      circle(d = $MountBore);
  translate([-15.5, 15.5])
      circle(d = $MountBore);
  translate([15.5, -15.5])
      circle(d = $MountBore);
  translate([-15.5, -15.5])
      circle(d = $MountBore);
}

module YPlateMotorHoles()
{
  rotate(90, [1, 0, 0])
    linear_extrude(height = 15, center = false, convexity = 10, twist = 0)
      YPlateMotorHoles2D();
}

module YPlate2D()
{
  translate([-9, -16])
  {
    intersection()
    {
      difference()
      {
        square([$XRailLength + 18, 86]);
        //Left mounting holes
        PlateHoleSet();
        translate([$XRailLength, 0, 0])
          PlateHoleSet();
        //Lower arch
        translate([($XRailLength / 2) + 9, (-700 / 2) + 30])
            circle(d = 700);
        //Y shaft bearing mounting
        translate([($XRailLength / 2) + 9, 57])
        {
          //Shaft opening
            circle(d = $ShaftDiameter);
          //Mounting screw holes
          YPlateMotorHoles2d($MountBore = $LockScrewBoreM3Laser + $PlateClearance);
        }
      }
      //Upper arch
      translate([($XRailLength / 2) + 9, (-550 / 2) + 78])
          circle(d = 550, h = 15);//, $fn = 200);
    }
  }
}

module YPlate()
{
  color($FrameAcrylicColor)
    linear_extrude(height = $AcrylicThickness, center = false, convexity = 10, twist = 0)
    {
      YPlate2D($ShaftDiameter = $ShaftDiameter);
    }
}

module YMotorMount()
{
  translate([21, -2, 21])
  {
    LeadScrewBearingBlock($Depth = 4);
    translate([0, -18, 0])
    {
      difference()
      {
        LeadScrewBearingBlock($Depth = 4);
        translate([-5, -3, 9])
          cube([10, 6, 15]);
      }
    }
    translate([20, -20, -66.5 - $HeightAdjust])
    difference()
    {
      cube([4, 22, 86.5 + $HeightAdjust]);
      translate([0, 7, 59])
        rotate(90, [0, 1, 0])
          LimitSwitchHoles();
    }
    
    translate([-24, -20, -66.5 - $HeightAdjust])
      cube([4, 22, 86.5 + $HeightAdjust]);
    
    translate([-24, -20, - $HeightAdjust - 66.5])
    difference()
    {
      cube([48, 4, 66.5]);
      translate([30, -1, 37])
        cube([15, 6, 25]);
    }
    //Base plate and mounting holes
    translate([-24, -20, -66.5 - $HeightAdjust])
    difference()
    {
      cube([48, 22, 4]);
      translate([24, 15, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 200);
      translate([38, 7, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 200);
      translate([10, 7, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 200);
    }
  }
}

module YMotorMountOld()
{
  translate([21, -2, 21])
  {
    LeadScrewBearingBlock($Depth = 4);
    translate([0, -18, 0])
    {
      difference()
      {
        LeadScrewBearingBlock($Depth = 4);
        translate([-5, -3, 9])
          cube([10, 6, 15]);
      }
    }
    translate([20, -20, -66.5 - $HeightAdjust])
    difference()
    {
      cube([4, 22, 86.5 + $HeightAdjust]);
      rotate(-23, [1, 0, 0])
        translate([-.1, 2, 0])
          cube([5, 22, 86.5]);
    }
    
    translate([-24, -20, -66.5 - $HeightAdjust])
    difference()
    {
      cube([4, 22, 86.5 + $HeightAdjust]);
      rotate(-23, [1, 0, 0])
        translate([-.1, 2, 0])
          cube([5, 22, 86.5]);
    }
    
  }
}

module YMotor()
{
  color([.2, .2, .2])
    cube([42, 42, 42]);
  color([.6, .6, .6])
    translate([21, 0, 21])
      rotate(90, [1, 0, 0])
        cylinder(d = 22, h = 3);
  color([.7, .7, .7])
    translate([21, 0, 21])
      rotate(90, [1, 0, 0])
        cylinder(d = 5, h = 21);
  color([.7, .7, .7])
    translate([21, -16, 21])
      rotate(90, [1, 0, 0])
        cylinder(d = 19, h = 25);
}

module YCarriagePlateMountHoles()
{
  translate([9, 2.5, -1])
    cylinder(d = $LockScrewBoreM2_5Laser + $PlateClearance, h = 10);
  translate([9, 12.5, -1])
    cylinder(d = $LockScrewBoreM2_5Laser + $PlateClearance, h = 10);
  translate([9, 42.5, -1])
    cylinder(d = $LockScrewBoreM2_5Laser + $PlateClearance, h = 10);
  translate([9, 52.5, -1])
    cylinder(d = $LockScrewBoreM2_5Laser + $PlateClearance, h = 10);
  //Rail fix screw holes
  translate([9, 7.5, -1])
    cylinder(d = 6.3, h = 10);
  translate([9, 47.5, -1])
    cylinder(d = 6.3, h = 10);
}

module YCarriagePlateMountHoles2D()
{
  translate([9, 2.5])
    circle(d = $LockScrewBoreM2_5Laser + $PlateClearance);
  translate([9, 12.5])
    circle(d = $LockScrewBoreM2_5Laser + $PlateClearance);
  translate([9, 42.5])
    circle(d = $LockScrewBoreM2_5Laser + $PlateClearance);
  translate([9, 52.5])
    circle(d = $LockScrewBoreM2_5Laser + $PlateClearance);
//  //Rail fix screw holes
//  translate([9, 7.5])
//    circle(d = 6.3);
//  translate([9, 47.5])
//    circle(d = 6.3);
}

module LimitSwitchHoles2D()
{
  translate([-5, 2.5])
    circle(d = $LockScrewBoreM2_5Laser);
  translate([-15, 2.5])
    circle(d = $LockScrewBoreM2_5Laser);
}

module YCoverPlate2D()
{
  difference()
  {
    //Base plate
  	square([$XRailSpacing + 18, 55]);
    //Y carriage mounting holes
    YCarriagePlateMountHoles2D();
    translate([$XRailSpacing, 0])
      YCarriagePlateMountHoles2D();
    //Motor mount openings
    translate([$XRailSpacing - 10, 32])
    {
    //Alignment collar
    translate([0, 0,0])
      hull()
      {
        translate([-5, 0])
          circle(d = 22);
        circle(d = 22);
      }
    //Mounting holes/screws. Note, longer so we can mount 2 different motor sizes
    translate([$XMotorMountSpacing + $XMotorMountExtension, $XMotorMountSpacing])
      hull()
      {
        translate([-5 - $XMotorMountExtension, 0])
          circle(d = 2.4);
        circle(d = 2.4);
      }
    translate([$XMotorMountSpacing + $XMotorMountExtension, -$XMotorMountSpacing])
      hull()
      {
        translate([-5 - $XMotorMountExtension, 0])
          circle(d = 2.4);
        circle(d = 2.4);
      }
    translate([-$XMotorMountSpacing, $XMotorMountSpacing])
      hull()
      {
        translate([-5 - $XMotorMountExtension, 0])
          circle(d = 2.4);
         circle(d = 2.4);
      }
    translate([-$XMotorMountSpacing, -$XMotorMountSpacing])
      hull()
      {
        translate([-5 - $XMotorMountExtension, 0])
          circle(d = 2.4);
        circle(d = 2.4);
      }
    }
    //Bearing shaft opening for bolt. No longer needed
//    translate([9, 26.5])
//      circle(d = 8);
    //Y rider mounting holes
    translate([(($XRailSpacing  + 18)/ 2) - 10, 5])
      circle(d = $LockScrewBoreM2_5Laser);
    translate([(($XRailSpacing  + 18)/ 2) + 10, 5])
      circle(d = $LockScrewBoreM2_5Laser);
    //Subtract limit switch mount holes
    translate([(($XRailLength  + 18)/ 2) + 40, 47])
//      rotate(180, [0, 1, 0])
        LimitSwitchHoles2D();
    
    //Belt clamp access opening
    translate([40, 16])
      square([26, 26]);
  }
}

module YCoverPlate()
{
  color($FrameAcrylicColor)
  linear_extrude(height = $AcrylicThickness, center = false, convexity = 10, twist = 0)
    YCoverPlate2D();
}

module YCoverPlate3D()
{
  color($FrameAcrylicColor)
  difference()
  {
    //Base plate
  	cube([$XRailLength + 18, 55, $AcrylicThickness]);
    //Y carriage mounting holes
    YCarriagePlateMountHoles();
    translate([$XRailLength, 0, 0])
      YCarriagePlateMountHoles();
    //Motor mount openings
    translate([$XRailLength - 10, 32, 2])
    {
    //Alignment collar
    translate([0, 0, -3])
      hull()
      {
        translate([-5, 0, 0])
          cylinder(d = 22, h = 5);
        cylinder(d = 22, h = 5);
      }
    //Mounting holes/screws
    translate([11.5, 11.5, -5])
      hull()
      {
        translate([-5, 0, 0])
          cylinder(d = 2.4, h = 70);
        cylinder(d = 2.4, h = 70);
      }
    translate([11.5, -11.5, -5])
      hull()
      {
        translate([-5, 0, 0])
          cylinder(d = 2.4, h = 70);
        cylinder(d = 2.4, h = 70);
      }
    translate([-11.5, 11.5, -5])
      hull()
      {
        translate([-5, 0, 0])
          cylinder(d = 2.4, h = 70);
         cylinder(d = 2.4, h = 70);
      }
    translate([-11.5, -11.5, -5])
      hull()
      {
        translate([-5, 0, 0])
          cylinder(d = 2.4, h = 70);
        cylinder(d = 2.4, h = 70);
      }
    }
    translate([9, 26.5, -1])
      cylinder(d = 8, h = 30);
    //Y rider mounting holes
    translate([(($XRailLength  + 18)/ 2) - 10, 5, -1])
      cylinder(d = $LockScrewBoreM2_5Laser + $PlateClearance, h = 30);
    translate([(($XRailLength  + 18)/ 2) + 10, 5, -1])
      cylinder(d = $LockScrewBoreM2_5Laser + $PlateClearance, h = 30);
    //Subtract limit switch mount holes
    translate([(($XRailLength  + 18)/ 2) + 40, 47, 9.46])
      rotate(180, [0, 1, 0])
        LimitSwitchHoles();
  }
}

module XStepper()
{
  //Main body
  color([0.3, 0.3, 0.3])
    translate([-14, -14, 0])
      cube([28, 28, 45]);
  //Shaft
  color([0.8, 0.8, 0.8])
    translate([0, 0, -20])
      cylinder(d = 5, h = 20);
  //Pulley
  translate([0, 0, 2.85 - $AcrylicThickness])
  color([0.8, 0.8, 0.8])
  {
    translate([0, 0, -12])
      cylinder(d = 12.9, h = 6);
    translate([0, 0, -12 - 7])
      cylinder(d = 9.6, h = 7);
    translate([0, 0, -12 - 7 - 1])
      cylinder(d = 12.9, h = 1);
  }
}

module LeadScrewRiderSmall()
{
  translate([0,  - 32.7, 0])
  color([0.8, 0.8, 0.83])
  {
    rotate(-90, [1, 0, 0])
      cylinder(d = 10, h = 15);
    translate([0, 1.6, 0])
      rotate(-90, [1, 0, 0])
      difference()
      {
        cylinder(d = 22, h = 3.6);
        translate([8, 0, -1])
        cylinder(d = 3.5, h = 6);
        translate([-8, 0, -1])
        cylinder(d = 3.5, h = 6);
        translate([0, 8, -1])
        cylinder(d = 3.5, h = 6);
        translate([0, -8, -1])
        cylinder(d = 3.5, h = 6);
      }
  }
}

module LeadScrewRiderLarge()
{
  translate([0,  - 32.7, 0])
  color([0.8, 0.8, 0.83])
  {
    rotate(-90, [1, 0, 0])
      cylinder(d = 15.7, h = 31);
    translate([0, 0, 0])
      rotate(-90, [1, 0, 0])
      difference()
      {
        cylinder(d = 30, h = 5);
        translate([11, 0, -1])
        cylinder(d = 3.5, h = 7);
        translate([-11, 0, -1])
        cylinder(d = 3.5, h = 7);
        translate([0, 11, -1])
        cylinder(d = 3.5, h = 7);
        translate([0, -11, -1])
        cylinder(d = 3.5, h = 7);
      }
  }
}

module LeadScrewRider()
{
  //LeadScrewRiderSmall();
  LeadScrewRiderLarge();
}

module BeltClamp()
{
  translate([-21, 5, 4.5])
  difference()
  {
    translate([0, 0, -1])
    cube([9, 6, 22]);
    translate([4.5, 8,9.5])
      rotate(90, [0, 0, 1])
        BeltClampHolePairVertical($Bore = $LockScrewBoreM2_5Vertical + $VerticalLockClearance + .5);
    translate([-1, 4.1, 8.5])
    cube([18, 2, 7]);
  }
  //Add gripper teeth
  for ($i = [1:4])
  {
    translate([($i * 2) - 21.5, 8, 12.2])
      rotate(45, [0, 0, 1])
        cube([1.5, 1.5, 8]);
  }
}

module RightSideRailCarriage()
{
  SideRailCarriage();
  if ($DoTall == 1)
  {
  translate([0, 0, 15])
    cube([18, 30, 20], center = true);
  }
}


module LeftSideRailCarriage()
{
  difference()
  {
    SideRailCarriage();
    translate([0, 0, ($HeightAdjust * $DoTall)])
    {
      //Bearing cut out
      translate([-10, -15.4, 11])
        cube([20, 3, 10.5]);
      translate([-10, -15.4, 21.4])
        rotate(-35, [1, 0, 0])
          cube([20, 3, 5]);
    }
  }
  translate([-9, -15.4,8])
    cube([18, 29, 4.4 + ($HeightAdjust * $DoTall)]);

  translate([0, 0, ($HeightAdjust * $DoTall)])
  {
  //Bearing lower mount
  translate([0, -1,8])
    cylinder(d1 = $BearingShaftDiameter, d2 = $BearingShaftDiameter, h = 13.5);
  translate([0, -1,21.5])
    cylinder(d1 = $BearingShaftDiameter, d2 = $BearingShaftDiameter - .5, h = 1);
  translate([0, -1,8])
    cylinder(d = $BearingShaftDiameter + 3, h = 4.9);
  }
}

module LeadScrewBlock()
{
  translate([0, -22.5, 2])
  {
    difference()
    {
      translate([0, 0, -3])
        cube([40, 10, 18.3], center = true);
      translate([0, 0, -0.15 - $AcrylicThickness])
      {
      //Y rider opening
      rotate(90, [1, 0, 0])
        cylinder(d = $LeadScrewAttachHoleDiameter, h = 20, center = true);
      //Y rider mounting screw holes
      translate([-11, 0, 0])
        rotate(90, [1, 0, 0])
          cylinder(d = $LockScrewBoreM3_5Vertical, h = 20, center = true);
      translate([11, 0, 0])
        rotate(90, [1, 0, 0])
          cylinder(d = $LockScrewBoreM3_5Vertical, h = 20, center = true);
      }
      //YPlate mounting holes
      translate([-10, 0, -20])
        cylinder(d = ($LockScrewBoreM2_5Horizontal) * $RoundToHex, h = 15, $fn = 6);
      translate([10, 0, -20])
        cylinder(d = ($LockScrewBoreM2_5Horizontal) * $RoundToHex, h = 15, $fn = 6);
    }
  }
}

module LeadScrewBearingBlock()
{
  difference()
  {
    cube([40, $Depth, 40], center = true);
    translate([0, 6, 0])
    {
      YPlateMotorHoles($MountBore = $LockScrewBoreM3Vertical + $HorizontalLockClearance);    
      rotate(90, [1, 0, 0])
        cylinder(d = $Bearing8mmDiameter, h = 15);
    }
  }
}

module BasePlateHoleSet()
{
  translate([7, 10])
   circle(d = $LockScrewBoreM3Laser);
  translate([33, 10])
   circle(d = $LockScrewBoreM3Laser);
} 

module BaseRowHoleSet()
{
  BasePlateHoleSet();
//  translate([0, $YRailLength])
//    BasePlateHoleSet();
  translate([0, $YRailLength - 7])
    BasePlateHoleSet();
}

module BasePlate()
{
  difference()
  {
    square([$XRailLength + 40, $YRailLength + 20]);
    BaseRowHoleSet();
    translate([$XRailLength, 0])
      BaseRowHoleSet();
//    translate([$XRailLength - 10, 0])
//      BaseRowHoleSet();
    translate([20, 10])
      AlignmentBarSet($Type = 1);
    //Y Motor support mounting holes
    translate([($XRailLength + 40 - 48) / 2, $YRailLength - 1.8])
    {
      translate([24, 15])
        circle(d = $LockScrewBoreM3Laser);
      translate([38, 7])
        circle(d = $LockScrewBoreM3Laser);
      translate([10, 7])
        circle(d = $LockScrewBoreM3Laser);
    }
  }
}

module AlignmentHoles()
{
  for ($i = [0:(($Length - 20) / 30)])
  {
    translate([5, ($i * 30) + 10, -1])
      cylinder(d = $LockScrewBoreM2Laser, h = 10);
  }
}

module AlignmentBar()
{
  if ($Type == 0)
  {
    square([10, $Length]);
  }
  else if ($Type == 1)
  {
    for ($i = [0:(($Length - 20) / 30)])
    {
      translate([5, ($i * 30) + 10])
        circle(d = $LockScrewBoreM2Laser);
    }
  }
  else if ($Type == 2)
  {
    for ($i = [0:(($Length - 20) / 30)])
    {
      translate([5, ($i * 30) + 10])
        circle(d = $LockScrewBoreM2Laser + $PlateClearance);
    }
  }
}

module AlignmentBarSet()
{
  //Left
  translate([10, 10])
    AlignmentBar($Length = $YRailLength - 60, $Type = $Type);
  translate([40, 10])
    AlignmentBar($Length = $YRailLength - 60, $Type = $Type);
//  translate([70, 10])
//    AlignmentBar($Length = $YRailLength - 60, $Type = $Type);
  //Right
//  translate([$XRailLength - 20, 10])
//    AlignmentBar($Length = $YRailLength - 60, $Type = $Type);
  translate([$XRailLength - 50, 10])
    AlignmentBar($Length = $YRailLength - 60, $Type = $Type);
  translate([$XRailLength - 80, 10])
    AlignmentBar($Length = $YRailLength - 60, $Type = $Type);
  //Top
//  translate([35, $YRailLength - 5])
//    rotate(-90, [0, 0, 1])
//      AlignmentBar($Length = $YRailLength - 70, $Type = $Type);
  translate([35, $YRailLength - 35])
    rotate(-90, [0, 0, 1])
      AlignmentBar($Length = $YRailLength - 70, $Type = $Type);
  translate([35, $YRailLength - 65])
    rotate(-90, [0, 0, 1])
      AlignmentBar($Length = $YRailLength - 70, $Type = $Type);
}

module AlignmentBars()
{
  difference()
  {
    AlignmentBarSet($Type = 0);
    AlignmentBarSet($Type = 2);
  }
}

module LimitSwitchHoles()
{
  translate([5, 2.5, -10])
    cylinder(d = $LockScrewBoreM2_5Laser, h = 30);
  translate([15, 2.5, -10])
    cylinder(d = $LockScrewBoreM2_5Laser, h = 30);
}

module LimitSwitch()
{
  difference()
  {
    color([0.3, 0.3, 0.3])
    cube([20, 10, 6.6]);
    LimitSwitchHoles();
  }
  color([0.7, 0.7, 0.7])
  translate([0, -1.8, 3.3])
  {
    translate([1.75, 0, 0])
      cube([0.5, 3.6, 3.4], center = true);
    translate([10, 0, 0])
      cube([0.5, 3.6, 3.4], center = true);
    translate([20-1.75, 0, 0])
      cube([0.5, 3.6, 3.4], center = true);
  }
  color([0.7, 0.7, 0.7])
    translate([3, 10, 1.3])
      rotate(10, [0, 0, 1])
        cube([17, .35, 4]);
}

module LeadScrewBearingBlockSupport()
{
  LeadScrewBearingBlock($Depth = 10);
  translate([-20, -5, $HeightAdjust - 105])
    cube([40, 10, 66]);
  difference()
  {
    translate([-30, -5, $HeightAdjust - 105])
      cube([60, 10, 10]);
    translate([25, 0, $HeightAdjust - 105])
      cylinder(d = $LockScrewBoreM3Horizontal + $HorizontalLockClearance, h = 20);
    translate([-25, 0, $HeightAdjust - 105])
      cylinder(d = $LockScrewBoreM3Horizontal + $HorizontalLockClearance, h = 20);
  }
}

module NewLaserCheck()
{
  translate([0, $YPosition, 0])
  {
    translate([$XPosition, 0, 0])
    {
      //Laser sled
      XCarriageFixedDiode();
    }
  }
  //Y carriage cover plate
  translate([-9, $YPosition - 27.5, 29])
    YCoverPlate();
  //X carriage belts
  color([0.1, 0.1, 0.1])
  {
    translate([0, $YPosition + 4.8 + 4.5, 13.4])
      cube([$XRailLength, 1.6, 6.2]);
    translate([0, $YPosition - 4.8 - 1.6 + 4.5, 13.4])
      cube([$XRailLength, 1.6, 6.2]);
  }
  //Belt clamps
  translate([$XPosition - 11.5, $YPosition, 0])
    BeltClamp();
  translate([$XPosition + 30.5, $YPosition, 0])
    BeltClamp();
  //Belt bearing
  translate([0, $YPosition - 1, 13])
    cylinder(d = 22, h = 7);

}

module FocusRing()
{
  difference()
  {
    cylinder(d = 40, h = 4, $fn = 12);
    cylinder(d = 12.4, h = 4, $fn = 40);
  }
}

module FocusRingNozzle()
{
  difference()
  {
    cylinder(d = 40, h = .5, $fn = 12);
    cylinder(d = 14, h = 4, $fn = 40);
  }
    translate([0, 0, .5])
  difference()
  {
    cylinder(d1 = 16, d2 = 7, h = 23, $fn = 40);
    cylinder(d1 = 14, d2 = 3, h = 23, $fn = 40);
  }
}

module DisplayAlignment()
{
  
  translate([0, $YPosition, 0])
  {
    
    //Y side rail carriages
    translate([0, 0, 	$HeightAdjust])
      LeftSideRailCarriage();
    translate([$XRailSpacing, 0, $HeightAdjust])
    {
      RightSideRailCarriage();
      translate([0, 27.5, 28])
        rotate(90, [0, 1, 0])
          rotate(-90, [1, 0, 0])
            LimitSwitch();
    }
    
    //Carrier rails
    translate([0, 0, 	$HeightAdjust])
      color([0.7, 0.7, 0.72])
      {
        translate([-10, -$CarrierSpacing / 2, $CarrierZOffset])
          rotate(90, [0, 1, 0])
            cylinder(d = 6, h = $XRailLength + 20);
        translate([-10, $CarrierSpacing / 2, $CarrierZOffset])
          rotate(90, [0, 1, 0])
            cylinder(d = 6, h = $XRailLength + 20);
      }
      //Laser sled
      translate([$XPosition, 0, $HeightAdjust])
      {
        XCarriageFixedDiode();
      }
    }
    
    //Y slider rails
    translate([0, 0, $HeightAdjust])
      color([0.7, 0.7, 0.72])
      {
        rotate(-90, [1, 0, 0])
          cylinder(d = 6, h = $YRailLength);
        translate([$XRailSpacing, 0, 0])
          rotate(-90, [1, 0, 0])
            cylinder(d = 6, h = $YRailLength);
      }
      
    //Base Y rail clamps
    YRailClamp();
    translate([$XRailLength, 0, 0])
      YRailClamp();
    translate([0, $YRailLength - 7, 0])
      YRailClamp();
    translate([$XRailLength, $YRailLength - 7, 0])
      YRailClamp();

  translate([0, 0, $HeightAdjust])
  {
    //Y travel end plates
    translate([0, -5, 0])
      rotate(90, [1, 0, 0])
        YPlate($ShaftDiameter = 23);
  //  translate([0, 5 + $AcrylicThickness, 0])
  //    rotate(90, [1, 0, 0])
  //      YPlate($ShaftDiameter = 23);
    translate([0, $YRailLength - 12, 0])
      rotate(90, [1, 0, 0])
        YPlate($ShaftDiameter = 23);
  //  translate([0, 5 + $AcrylicThickness + $YRailLength, 0])
  //    rotate(90, [1, 0, 0])
  //      YPlate($ShaftDiameter = 23);
    //Y leadscrew
    color([0.8, 0.8, 0.83])
      translate([$XRailLength / 2, 0, 41])
        rotate(-90, [1, 0, 0])
          cylinder(d = 8, h = $YRailLength);
    //Y leadscrew rider
    translate([$XRailLength / 2, $YPosition, 41])
      LeadScrewRider();
    //Y movement motor
    translate([($XRailLength - 42) / 2, $YRailLength + 7 + $AcrylicThickness, 19.5])
    {
      YMotor();
      YMotorMount();
    }
    //Y carriage cover plate
    translate([-9, $YPosition - 27.5, 29])
      YCoverPlate();
    
    //X carriage stepper
    translate([$XRailLength - 25, $YPosition + 4.5, 29 + $AcrylicThickness])
      XStepper();
      
      
    //X carriage belts
    color([0.1, 0.1, 0.1])
    {
      translate([0, $YPosition + 4.8 + 4.5, 13.4])
        cube([$XRailLength, 1.6, 6.2]);
      translate([0, $YPosition - 4.8 - 1.6 + 4.5, 13.4])
        cube([$XRailLength, 1.6, 6.2]);
    }

    //Belt clamps
    translate([$XPosition - 11.5, $YPosition, 0])
      BeltClamp();
    translate([$XPosition + 30.5, $YPosition, 0])
      BeltClamp();
   
    //Belt bearing
    translate([0, $YPosition - 1, 13])
      cylinder(d = 22, h = 7);
    //Y leadscrew attatch
    translate([$XRailLength / 2, $YPosition, 39.15 + $AcrylicThickness])
      LeadScrewBlock();
    //Leadscrew support block. Note, thinner than the rail supports so that the plates are not parallel, hence stronger.
    translate([$XRailLength / 2, 0, 41])
      LeadScrewBearingBlockSupport();
    
  //  //Motor support block
  //  translate([$XRailLength / 2, $YRailLength, 41])
  //    LeadScrewBearingBlock($Depth = 8);
      //Y travel limit switch
    translate([($XRailLength / 2) + 14, $YRailLength, -6])
      rotate(90, [0, 1, 0])
        rotate(180, [0, 0, 1])
          LimitSwitch();
    //Laser focus ring
    translate([$XPosition-7, $YPosition + 6, -5])
      FocusRing();
  }
  
  //Base plate
  color($BaseAcrylicColor)
    translate([-20, -10, -26 - $BaseAcrylicThickness])
      linear_extrude(height = $BaseAcrylicThickness, center = false, convexity = 10, twist = 0)
        BasePlate();
  //PCB alignment bars
  color($AlignmentAcrylicColor)
    translate([0, 0,  -26.01])
      linear_extrude(height = $AlignmentAcrylicThickness, center = false, convexity = 10, twist = 0)
        AlignmentBars();


}

module HoleChecker()
{
  difference()
  {
    translate([5, -10])
    square([255, 20]);
    for ($i = [10 : 50])
    {
      translate([($i * $i) / 10, 0, 0])
      circle(d = $i / 10);
    }
  }
}

module PrintItems()
{
  
  //Leadscrew attach clamp
  translate([10, 63, 27.5])
    rotate(90, [1, 0, 0])
      rotate(90, [0, 1, 0])
        LeadScrewBlock();

  //Base Y rail clamps : 4x
    translate([5, 100, 26])
      rotate(90, [0, 0, 1])
      YRailClamp();
    translate([16, 100, 26])
      rotate(90, [0, 0, 1])
      YRailClamp();
    translate([16, 122, 26])
      YRailClamp();
    translate([96, 122, 26])
      YRailClamp();
  
  translate([119, 74, 10])
    LeftSideRailCarriage();
  translate([99, 74, 10])
    RightSideRailCarriage();
    
  translate([55, 75, 0])
    rotate(90, [0, 0, 1])
      XCarriageFixedDiode();
  
  //Note, thinner than the rail supports so that the plates are not parallel, hence stronger.
  translate([62, 20, 4])
    rotate(90, [1, 0, 0])
      LeadScrewBearingBlock($Depth = 8);
  translate([20, 20, 4])
    rotate(90, [1, 0, 0])
      LeadScrewBearingBlock($Depth = 8);
      

  translate([105, 128, -5])
    rotate(90, [0, 0, 1])
    rotate(90, [1, 0, 0])
      BeltClamp();
  translate([80, 128, -5])
    rotate(90, [0, 0, 1])
    rotate(90, [1, 0, 0])
      BeltClamp();
   
  translate([88, 41, 22])
    rotate(90, [1, 0, 0])    
      YMotorMount();      
      
  translate([60, 107, 0])
    FocusRing(); 
    
  translate([25, 127, 0])
    YDial();
}

module CalibrationPrint()
{
  //Vertical openings
  difference()
  {
    cube([50, 20, 10]);
    //Vertical locking holes
    translate([5, 5, -1])
      cylinder(d = $LockScrewBoreM2_5Vertical, h = 12, $fn = 50);
    translate([15, 5, -1])
      cylinder(d = $LockScrewBoreM3Vertical, h = 12, $fn = 50);
    translate([27, 10, -1])
      cylinder(d = $LockScrewBoreM8Vertical, h = 12, $fn = 50);
    //Vertical free holes
    translate([5, 15, -1])
      cylinder(d = $LockScrewBoreM2_5Vertical + $VerticalLockClearance, h = 12, $fn = 50);
    translate([15, 15, -1])
      cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 12, $fn = 50);
    //Leadscrew traveller
    translate([40, 10, -1])
        cylinder(d = $LeadScrewAttachHoleDiameter, h = 20);
  }
  //Horizontal openings
  translate([50, 0, 0])
  difference()
  {
    cube([30, 10, 20]);
    //3MM shaft
    translate([5, 20, 5])
      rotate(90, [1, 0, 0])
        cylinder(d = $Shaft3mmHorizontal * $RoundToHex, h = 70, $fn = 6);
    //M2.5 locking
    translate([15, 20, 5])
      rotate(90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal * $RoundToHex, h = 70, $fn = 6);
    //M3 locking
    translate([25, 20, 5])
      rotate(90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM3Horizontal * $RoundToHex, h = 70, $fn = 6);
    //Horizontal free holes
    //M2.5 free
    translate([15, 20, 15])
      rotate(90, [1, 0, 0])
        cylinder(d = ($LockScrewBoreM2_5Horizontal  + $HorizontalLockClearance)* $RoundToHex, h = 70, $fn = 6);
    //M3 free
    translate([25, 20, 15])
      rotate(90, [1, 0, 0])
        cylinder(d = ($LockScrewBoreM3Horizontal + $HorizontalLockClearance) * $RoundToHex, h = 70, $fn = 6);
  }
  //Bearing mount
  translate([89, 5, 10])
    RodClamp(width = 18, height = 20, screwoffset = 0, depth = 10, boreheight = 10, boreradius = ($Bearing6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 1.5);
  translate([105, 5, 6])
    RodClamp(width = 15, height = 20, screwoffset = 0, depth = 10, boreheight = 6, boreradius = ($Rail6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 0);
  //Bearing piller
  translate([24, 20, 0])
    cylinder(d = $BearingShaftDiameter + 3, h = 20);
  
  //Bearing/motor block
  translate([0, 20, 0])
  difference()
  {
    cube([30, 30, 10]);
    translate([15, 15, -1])
        cylinder(d = $Bearing8mmDiameter, h = 15);
  }
  
}

module AlignmentBarPiece()
{
  difference()
  {
    AlignmentBar($Length = $YRailLength - 60, $Type = 0);  
    AlignmentBar($Length = $YRailLength - 60, $Type = 2);  
  }
}

module LaserSet1()
{
  //Set 1
  BasePlate();
}

module LaserSet2()
{
  //Set 2
  translate([0, 0])
    YCoverPlate2D();
  translate([9, 73])
    YPlate2D($ShaftDiameter = 23);
  translate([9, 123])
    YPlate2D($ShaftDiameter = 23);
  translate([9, 173])
    YPlate2D($ShaftDiameter = 23);
  translate([9, 223])
    YPlate2D($ShaftDiameter = 23);
}

module LaserSet3()
{
  //Set 3
  translate([0, 0])
    AlignmentBarPiece();
  translate([11, 0])
    AlignmentBarPiece();
  translate([22, 0])
    AlignmentBarPiece();
}

module LaserAll()
{
  LaserSet1();
  translate([350, 0])
    LaserSet2();
  translate([670, 0])
    LaserSet3();  
}

module PrintNewBits()
{
XCarriageFixedDiode();

translate([60, -7, 0])
  rotate(180, [0, 0, 1])
    translate([30, 0, 5])
      rotate(90, [1, 0, 0])
	    YRailClampTall();

translate([-27, 4, 0])
  rotate(180, [0, 0, 1])
    translate([30, 0, 5])
      rotate(90, [1, 0, 0])
	    YRailClampTall();

translate([-20, 10, 0])
  rotate(90, [0, 0, 1])
    translate([30, 0, 5])
      rotate(90, [1, 0, 0])
	    YRailClampTall();

translate([-20, 10, 0])
  rotate(90, [0, 0, 1])
    translate([30, 0, 5])
      rotate(90, [1, 0, 0])
	    YRailClampTall();

translate([-20, -70, 0])
  rotate(90, [0, 0, 1])
    translate([30, 0, 5])
      rotate(90, [1, 0, 0])
	    YRailClampTall();


}

module LaserFocusTest()
{
	difference()
	{
		cube([18, 18, 45], center = true);
		cylinder(d = 12.5, h = 48, center = true);
  }
}

module YDial()
{
  translate([0, 0, 1.5])
  difference()
  {
    union()
    {
      minkowski()
      {
        cylinder(d = 35, h = 12, $fn = 40);
        sphere(d = 3);
      }
      cylinder(d = 15, $fn = 40, h = 22);
    }
    translate([0, 0, 2])
      cylinder(d = 8.3, h = 25);
    translate([0, 0, 18])
      rotate(90, [1, 0, 0])
        cylinder(d = LockScrewBoreM2_5Horizontal, h = 20);
  }
}

/////////////////////////////////////////////////////////////////////////

//PrintItems();

//LaserAll(); 
//LaserSet3(); 

//DisplayAlignment();

//HoleChecker();

//CalibrationPrint();

//NewLaserCheck();

//PrintNewBits();

//XCarriageFixedDiode();

//LaserFocusTest();

//BasePlate();

//translate([-25, -10, 5])
//  rotate(90, [1, 0, 0])
//    LeadScrewBearingBlockSupport();

//translate([0, 0, 22])
//rotate(90, [1, 0, 0])
//  YMotorMount();

//FocusRing();
FocusRingNozzle();
  

  
  