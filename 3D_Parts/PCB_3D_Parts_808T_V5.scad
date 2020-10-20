$fn = 100;

//Main parameters to tune : Configured for MatterControl
$Rail6mmDiameter = 6.35;//Hole for X 6mm rail attachments to Y carriages and Y rails to base supports
$LeadScrewAttachHoleDiameter = 16.3;//Leadscrew block hole for T-threads
$LeadScrewBore = 8.3;//Lead screw attach bore for coupler and Y dial
$YMotorShaftDiameter = 4.9;//Bore for Y motor attach to coupler
$YLeadscrewDiameter = 4.9;//Bore for Y leadscrew attach to coupler
$VerticalLockClearance = .4;//Delta between vertical threaded hole and free hole
$HorizontalLockClearance = .35;//Delta between horizontal threaded hole and free hole
$LockScrewBoreM2_5Vertical = 2.4;//3D printed vertical hole for M2.5 threadlock - Good
$LockScrewBoreM2_5Horizontal = 2.3;//3D printed horizontal hole for M2.5 threadlock
$LockScrewBoreM3Vertical = 3.0;
$LockScrewBoreM3_5Vertical = 3.3;
$LockScrewBoreM3Horizontal = 2.85;
$LockScrewBoreM8Vertical = 7.8;//Vertical M8 threadlocked hole - Good
$Shaft3mmHorizontal = 3.05;
$Bearing8mmDiameter = 22.2;
$MotorMountDiameter = 24;
$ThreadedBarSupportDiameter = 5.5;
$LockScrewBoreM2_5Laser = 2.2;//Laser mounting holes for leadscrew block and limit switches
$LockScrewBoreM3Laser = 2.7;
$LockScrewBoreM2Laser = 1.7;//Laser hole size for M2 thread locking
$Bearing6mmDiameter = 12.55;//Bearing opening to take 6mm bearing. Make just a little slack then tighten with screw or glue
$BearingShaftDiameter = 8.0;
$DiodeHousingHole = 12.45;
$DiodeHostWidth = 13;
$DiodeHostLength = 15.5;
$YScrewBearingDiameter = 10 + .5;

$AcrylicThickness = 3.0;
$BaseAcrylicThickness = 10;
$AlignmentAcrylicThickness = 1.5;

$PlateClearance = .15;

$XMotorMountExtension = 10; // Extend the X motor Y plate mounting hole lengths to accomodate different motors and allow tensioning of the belt
$FanAdder = 20;//Additional clearance required for cooling fan
$HeightAdjust = 14;//Allow adjustment for overall carriage height for 808T focus requierments. 19 is good since it aligns with original y plate holes to allow dual fitting
$DoTall = 0;

$BaseAcrylicColor = [0.1, 0.5, 0.0, 0.9];
$FrameAcrylicColor = [0.7, 0.8, 0.2, 0.9];
$AlignmentAcrylicColor = [0.3, 0.7, 0.7, 0.4];
$BaseAcrylicColor = [0.1, 0.1, 0.5, 0.6];

$YPosition = 100;
$XPosition = 265;

$RoundToHex = 1.155;

$XRailSpacing = 300;
$CarrierSpacing = 45;
$CarrierZOffset = 14;
$LaserCarrierWidth = 58;
$LaserCarrierLength = 68;
$XRailLength = 300;
$YRailLength = 270;

//XMotor parameters
//Nema 11
//$XStepperMotorWidth = 28;
//$XStepperMotorDepth = 45;
//$XStepperMotorSpacing = 23;
//Nema 17
$XStepperMotorWidth = 43;
$XStepperMotorDepth = 39;
$XStepperMotorSpacing = 31;
//Nema 11 = 11.5, Nema 17 = 15.5
//Use a setting between the 2 to allow either motor to be used
$XMotorMountSpacing = 15.5;

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

module SideRailClamp()
{
  difference()
  {
	translate([0, 0, ($HeightAdjust * $DoTall)])
    rotate(90, [0, 0, 1])
      RodClamp(width = 15, height = 21 + ($HeightAdjust * $DoTall) + $FanAdder - 6, screwoffset = 0, depth = 18, boreheight = 6 + ($HeightAdjust * $DoTall), boreradius = ($Rail6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 0);
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
    translate([0, 5, -5 + $FanAdder])
      cylinder(d = $LockScrewBoreM2_5Vertical, h = 25);
    translate([0, -5, -5 + $FanAdder])
      cylinder(d = $LockScrewBoreM2_5Vertical, h = 25);
	}
  }
}

module SideRailCarriage()
{
  //Lower side rail rider for bearing
  difference()
  {
    RodClamp(width = 18, height = 18, screwoffset = 0, depth = $CarrierSpacing + 15, boreheight = 10, boreradius = ($Bearing6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 1.5);
    //Lock screw holes
    //Center
    translate([-15, 0, 0])
      rotate(90, [0, 1, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 30);
    //Left
    translate([-15, 20, 0])
      rotate(90, [0, 1, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 30);
    //Right
    translate([-15, -20, 0])
      rotate(90, [0, 1, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 30);
  }
  difference()
  {
    union()
    {
      translate([0, -$CarrierSpacing / 2, $CarrierZOffset])
        SideRailClamp();
      translate([0, $CarrierSpacing / 2, $CarrierZOffset])
        SideRailClamp();
    }
    translate([0, -($CarrierSpacing / 2), 16.5 + $FanAdder])
      rotate(90, [0, 1, 0])
        rotate(-90, [1, 0, 0])
          LimitSwitchHoles($Length = 30);
  }
}

module BeltClampHolePairVertical()
{
  translate([0, 0, -3])
    rotate(-90, [0, 1, 0])
      cylinder(d = $Bore, h = 20);
  translate([0, 0, 8])
    rotate(-90, [0, 1, 0])
      cylinder(d = $Bore, h = 20);
}

module YRailClamp()
{
  difference()
  {
    translate([0, 0, $HeightAdjust])
      RodClamp(width = 18, height = $HeightAdjust + 30, screwoffset = 2, depth = 10, boreheight = $HeightAdjust + 20, boreradius = ($Rail6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 1, captivestyle = 1, captivebore =0, captivedepth = 1.5);
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
    }
  
    //Lock screw holes
    translate([-20, 0, $HeightAdjust])
      rotate(90, [0, 1, 0])
        cylinder(d = $LockScrewBoreM2_5Vertical, h = 40);
    //Limit switch holes
    translate([0, -2.5, 25])
    rotate(90, [0, 1, 0])
    LimitSwitchHoles($Length = 30);
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

module YMotorSideMount()
{
  translate([0, 12, 40])
  {
    translate([0, 0, -2])
      MotorMountBlock($Depth = 4, $Align = 0);
    //Side supports
    translate([20, -38, -40])
    difference()
    {
      cube([4, 40, 35]);
      translate([-.1, 0, 4])
        rotate(42, [1, 0,0])
          cube([4.2, 60, 40]);
    }   
    translate([-24, -38, -40])
    difference()
    {
      cube([4, 40, 35]);
      translate([-.1, 0, 4])
        rotate(44, [1, 0,0])
          cube([4.2, 60, 40]);
    }
    
    //Back support
    translate([-24, -2, -40])
      cube([48, 4, 20]);

    //Base plate and mounting holes
    translate([-24, -40, -40])
    difference()
    {
      translate([0, 2, 0])
        cube([48, 40, 4]);
      translate([38, 7, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 200);
      translate([10, 7, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 200);
      translate([38, 30, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 200);
      translate([10, 30, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 200);
    }
  }
}

module YMotorAndShaftMount()
{
  translate([15, 5, -26])
    rotate(180, [0, 0, 1])
      YMotorSideMount();
//  YRailClamp();
    translate([0, 0, $HeightAdjust])
      difference()
      {
        RodClamp(width = 18, height = $HeightAdjust + 30, screwoffset = 2, depth = 10, boreheight = $HeightAdjust + 20, boreradius = ($Rail6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 1, captivestyle = 1, captivebore =0, captivedepth = 1.5);
        translate([-15, -20, -29])
          rotate(-45, [1, 0, 0])
            cube([30, 30, 40]);
        translate([0, 0, -$HeightAdjust])
        {
          //Lock screw hole
          translate([0, 0, $HeightAdjust])
              cylinder(d = $LockScrewBoreM2_5Horizontal, h = 20);
          //Limit switch holes
          translate([0, -2.5, 23])
            rotate(90, [0, 1, 0])
              LimitSwitchHoles($Length = 8);
        }
        translate([7, -12, -15])
        cube([10, 20, 40]);
      }
    //Fill in to allow face down printing
    translate([-9, -9, 9])
      cube([10, 4, 15]);
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

module YCarriagePlateMountHoles2D()//$CarrierSpacing
{
  translate([9, $CarrierSpacing - 45])
    circle(d = $LockScrewBoreM2_5Laser + $PlateClearance);
  translate([9, $CarrierSpacing - 35])
    circle(d = $LockScrewBoreM2_5Laser + $PlateClearance);
  translate([9, $CarrierSpacing ])
    circle(d = $LockScrewBoreM2_5Laser + $PlateClearance);
  translate([9, $CarrierSpacing + 10])
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

module XPulley()
{
  difference()
  {
    translate([0, 0, 3.7 - $AcrylicThickness])
    {
      color([0.8, 0.8, 0.8])
      {
      translate([0, 0, -12.4])
        cylinder(d = 12.9, h = $Height - 7.4);
      translate([0, 0, -12 - 7])
        cylinder(d = 9.6, h = 6.6);
      translate([0, 0, -12 - 7 - 1])
        cylinder(d = 12.9, h = 1);
      }
      //Depth gauge marker
      translate([0, 0, -16 ])
      color([.8, .4, .2])
        cylinder(d = 11.4, h = 2);
    }
    translate([0, 0, -25])
    cylinder(d = $Shaft, h = $Height + 10);
  }
}

module Stepper()
{
  //Main body
  color([0.3, 0.3, 0.3])
    translate([-Width / 2, -Width / 2, 0])
      cube([Width, Width, Depth]);
  //Shaft
  color([0.8, 0.8, 0.8])
    translate([0, 0, -20])
      cylinder(d = 5, h = 20);
  echo("Pulley type = ", $Pulley);
  //Pulley
  if ($Pulley == 1)
  {
    XPulley(Height = 14, Shaft = 0);
  }
  else if ($Pulley == 2)
  {
    color([.3, .5, .7])
      translate([0, 0, -12])
        cylinder(d = 6.5, h = 10);
  }
  
  //Mounting holes
  translate([-Spacing / 2, -Spacing / 2, -15])
    cylinder(d = 3, h = 15);
  translate([Spacing / 2, -Spacing / 2, -15])
    cylinder(d = 3, h = 15);
  translate([-Spacing / 2, Spacing / 2, -15])
    cylinder(d = 3, h = 15);
  translate([Spacing / 2, Spacing / 2, -15])
    cylinder(d = 3, h = 15);
}

module LeadScrewRiderSmall()
{
  translate([0,  - 32.5, 0])
  color([0.8, 0.8, 0.83])
  {
    rotate(-90, [1, 0, 0])
      cylinder(d = 10, h = 15);
    translate([0, 15, 0])
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
  translate([0,  - 43, 0])
  color([0.8, 0.8, 0.83])
  {
    rotate(-90, [1, 0, 0])
      cylinder(d = 15.7, h = 31);
    translate([0, 26, 0])
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
    translate([0, 0, 3])
    cube([9, 6, 18]);
    translate([4.5, 8,9.5])
      rotate(90, [0, 0, 1])
        BeltClampHolePairVertical($Bore = $LockScrewBoreM2_5Vertical + $VerticalLockClearance + .5);
    translate([-1, 4.4, 8.5])
    cube([18, 2, 7]);
  }
  
  //Add gripper teeth
  for ($i = [1:4])
  {
    translate([($i * 2) - 21.5, 8.2, 12.9])
      rotate(45, [0, 0, 1])
        cube([1.5, 1.5, 7.4]);
  }
}

module RightSideRailCarriage()
{
  difference()
  {
    SideRailCarriage();
    //Chop off extended support pillar
    translate([0, ($CarrierSpacing / 2), 40])
    cube([20, 20, 40], center = true);
  }
  //Add angled support
  translate([-9, ($CarrierSpacing / 2)-7.5, 20])
  {
    difference()
    {
      //Main stem
      rotate(-39.4,[1, 0, 0])
        cube([18, 15, 32]);
      //Bottom edge,around bearing opening
      translate([-.5, 15, -2.95])
        rotate(169, [1, 0, 0])
          cube([19, 20, 10]);
      //Top
      translate([-.5, 13, 23])
        cube([19, 25, 15]);
      //Support bar opening
      translate([-1, 20.5, 12])
        rotate(30, [1, 0, 0])
          rotate(90, [0,1, 0])
            cylinder(d = $ThreadedBarSupportDiameter * $RoundToHex, h = 20, $fn = 6); 
      //Lock hole
      translate([9, 29.5, 5])
        rotate(50, [1, 0, 0])
          cylinder(d = $LockScrewBoreM2_5Vertical, h = 10);
      
    }
    
    //Now add top support section
    translate([0, 14.4, 15.2])
    {
      difference()
      {
        cube([18, 17.5, 7.8]);
        translate([0, 17, 0])
        {
          translate([9, -2, -25 + $FanAdder])
            cylinder(d = $LockScrewBoreM2_5Vertical, h = 25);
          translate([9, -12, -25 + $FanAdder])
            cylinder(d = $LockScrewBoreM2_5Vertical, h = 25);
        }
      }
    }
  }
  
  //Limit switch bumper
  difference()
  {
    translate([8, ($CarrierSpacing / 2) + 4.5, -10])
      cube([9, 8, 20]);
    translate([11.5, ($CarrierSpacing / 2) + 4.5, -10.5])
      rotate(-15, [0, 0, 1])
        cube([9, 25, 21]);
    translate([-1.15, ($CarrierSpacing / 2) + 4.5, -10.5])
      rotate(-15, [0, 0, 1])
        cube([9, 25, 21]);
  }

  //Y threaded anti-backlash
  translate([-15, 20, -2])
    YBacklash();
  
  if ($DoTall == 1)
  {
  translate([0, 0, 15])
    cube([18, 30, 20], center = true);
  }
}


module LeftSideRailCarriage()
{

  //Lower side rail rider for bearing
  difference()
  {
    RodClamp(width = 18, height = 18, screwoffset = 0, depth = $CarrierSpacing + 15, boreheight = 10, boreradius = ($Bearing6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 1.5);
    //Lock screw holes
    //Center
    translate([-15, 0, 0])
      rotate(90, [0, 1, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 30);
    //Left
    translate([-15, 20, 0])
      rotate(90, [0, 1, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 30);
    //Right
    translate([-15, -20, 0])
      rotate(90, [0, 1, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 30);
  }
  difference()
  {
    union()
    { 
      translate([0, -$CarrierSpacing / 2, $CarrierZOffset])
      {
        difference()
        {
          translate([0, 0, ($HeightAdjust * $DoTall)])
            rotate(90, [0, 0, 1])
              RodClamp(width = 15, height = 20, screwoffset = 0, depth = 18, boreheight = 6 + ($HeightAdjust * $DoTall), boreradius = ($Rail6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 0);
          translate([0, 0, ($HeightAdjust * $DoTall)])
          {
            //Rail clamp hole
            translate([0, 10, 0])
              rotate(90, [1, 0, 0])
                cylinder(d = $LockScrewBoreM2_5Horizontal, h = 20);
          }
        }
      }
      //Belt bearing block
      difference()
      {
        union()
        {
          translate([-9, ($CarrierSpacing / 2) -38,8])
            cube([18, $CarrierSpacing - 14, 5 + ($HeightAdjust * $DoTall) + $FanAdder - 6]);
        //Bearing shaft Spacer
        translate([0, 8.5,($HeightAdjust * $DoTall) + $FanAdder - 6])
          cylinder(d = $LockScrewBoreM3Vertical + 1.5, h = 13.7);
        }
        //Bearing shaft opening
        translate([0, 8.5,($HeightAdjust * $DoTall) + $FanAdder - 6])
          cylinder(d = $LockScrewBoreM3Vertical, h = 20); 
      }
    }
    //Remove excess to create slope
    translate([-10, -30, 20])
      rotate(13, [1, 0, 0])
        cube([20, 40, 20]);
  }
  
  //X shaft clamp
  translate([0, $CarrierSpacing / 2, $CarrierZOffset])
  {
    difference()
    {
      translate([0, 0, ($HeightAdjust * $DoTall)])
        rotate(90, [0, 0, 1])
          RodClamp(width = 15, height = 19, screwoffset = 0, depth = 18, boreheight = 6 + ($HeightAdjust * $DoTall), boreradius = ($Rail6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 0);
      translate([0, 0, ($HeightAdjust * $DoTall)])
      {
        //Rail clamp hole
        translate([0, 10, 0])
          rotate(90, [1, 0, 0])
            cylinder(d = $LockScrewBoreM2_5Horizontal, h = 20);
      }
    }
  }
  
  //Add angled support
  translate([-9, ($CarrierSpacing / 2)-7.5, 20])
  {
    difference()
    {
      //Main stem
      rotate(-39.4,[1, 0, 0])
        cube([18, 15, 35]);
      //Bottom edge,around bearing opening
      translate([-.5, 15, -2.95])
        rotate(169, [1, 0, 0])
          cube([19, 20, 10]);
      //Slice off top
      translate([-.5, 13, 18])
        cube([19, 25, 15]);
      //Slice side top
      translate([-.5, 28, 7])
        cube([19, 20, 15]);
      //Lock hole
      translate([9, 32, 3])
        rotate(50, [1, 0, 0])
          cylinder(d = $LockScrewBoreM2_5Vertical, h = 16);
      //Support bar opening
      translate([-1, 20.5, 12])
        rotate(30, [1, 0, 0])
          rotate(90, [0,1, 0])
            cylinder(d = $ThreadedBarSupportDiameter * $RoundToHex, h = 20, $fn = 6); 
    }
  }

  //Y threaded anti-backlash
  translate([15, 20, -2])
    YBacklash();
}

module MotorMountHoles()
{
  YPlateMotorHoles($MountBore = $LockScrewBoreM3Vertical + $HorizontalLockClearance);    
  rotate(90, [1, 0, 0])
    cylinder(d = $MotorMountDiameter, h = 15);
}

module MotorMountBlock()
{
  difference()
  {
    cube([40, $Depth, 40], center = true);
    translate([0, 6, 0])
    {
      MotorMountHoles();
    }
  }
  if ($Align == 1)
    rotate(90, [1, 0, 0])
      color([0.2, 0.6, 0.8, 0.5])
        cylinder(d = 5, h = 300);
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
  translate([0, $YRailLength - 7])
    BasePlateHoleSet();
}

module MotorAndSupportHoles()
{
  //Y Motor support mounting holes
  translate([($XRailLength + 40 - 48) / 2, $YRailLength - 22])
  {
    translate([38, 7, -1])
      circle(d = $LockScrewBoreM3Laser);
    translate([10, 7, -1])
      circle(d = $LockScrewBoreM3Laser);
    translate([38, 30, -1])
      circle(d = $LockScrewBoreM3Laser);
    translate([10, 30, -1])
      circle(d = $LockScrewBoreM3Laser);
  }
  
  //Leadscrew bearing support
  translate([(($XRailLength / 2) + 20) + 25, 10])
    circle(d = $LockScrewBoreM3Laser);
  translate([(($XRailLength / 2) + 20) -25, 10])
    circle(d = $LockScrewBoreM3Laser);
}

module BasePlateHoles()
{
  BaseRowHoleSet();
  translate([$XRailLength, 0])
    BaseRowHoleSet();
  translate([20, 10])
    AlignmentBarSet($Type = 1);
  MotorAndSupportHoles();
  translate([65, 0, 0])
    MotorAndSupportHoles();
  translate([-65, 0, 0])
    MotorAndSupportHoles();
}

module BasePlate()
{
  difference()
  {
    square([$XRailLength + 40, $YRailLength + 20]);
    BasePlateHoles();
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

  //Right
  translate([$XRailLength - 50, 10])
    AlignmentBar($Length = $YRailLength - 60, $Type = $Type);
  translate([$XRailLength - 80, 10])
    AlignmentBar($Length = $YRailLength - 60, $Type = $Type);

  //Top
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
    cylinder(d = $LockScrewBoreM2_5Laser, h = $Length);
  translate([15, 2.5, -10])
    cylinder(d = $LockScrewBoreM2_5Laser, h = $Length);
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

module YCarriageCheck()
{
  translate([0, $YPosition, 0])
  {
    //X carriage stepper
    translate([$XRailLength , 8.5, 29 + $AcrylicThickness + $FanAdder - 6])
    {
      Stepper(Width = $XStepperMotorWidth, Depth = $XStepperMotorDepth, Spacing = $XStepperMotorSpacing, $Pulley = 1);
      XCarriageMotorMount();
    }

    //Y side rail carriages
    LeftSideRailCarriage();
    translate([$XRailSpacing, 0, 0])
    {
      RightSideRailCarriage();
      translate([0, -($CarrierSpacing / 2) - 14, 14 + $FanAdder])
        rotate(90, [0, 1, 0])
          rotate(-90, [1, 0, 0])
            LimitSwitch();
    }
    
    //Carrier rails
    color([0.7, 0.7, 0.72])
    {
      //Linear rails
      translate([-10, -$CarrierSpacing / 2, $CarrierZOffset])
        rotate(90, [0, 1, 0])
          cylinder(d = 6, h = $XRailLength + 20);
      translate([-10, $CarrierSpacing / 2, $CarrierZOffset])
        rotate(90, [0, 1, 0])
          cylinder(d = 6, h = $XRailLength + 20);
      //Threaded rod strengthener
      translate([-10,  ($CarrierSpacing / 2) + 13, $CarrierZOffset + 18])
        rotate(90, [0, 1, 0])
          cylinder(d = 5, h = $XRailLength + 20);
    }
    
    translate([$XPosition, 0, 0])
    {
      //Laser sled
      XCarriageSquareHost2();
    }
  }
  //X carriage belts
  translate([0, 0, -6])
  {
    color([0.1, 0.1, 0.1])
    {
      translate([-10, $YPosition + 4.8 + 8.5, 13.4 + $FanAdder + 1])
        cube([$XRailLength + 10, 0.8, 6.2]);
      translate([-10, $YPosition - 4.8 - 0.8 + 8.5, 13.4 + $FanAdder + 1])
        cube([$XRailLength + 10, 0.8, 6.2]);
    }
    //Belt guides to help align inner teeth
    color([0.5, 0.5, 0.5])
    {
      translate([-10, $YPosition + 4.8 + 8.5 + -0.8, 15.4 + $FanAdder + 1])
        cube([$XRailLength + 10, 1.6, 3]);
      translate([-10, $YPosition - 4.8 -0.8 + 8.5, 15.4 + $FanAdder + 1])
        cube([$XRailLength+10, 1.6, 3]);
    }
  }
  //Belt clamps
  translate([$XPosition - 39, $YPosition + 14,  $FanAdder - 5])
    rotate(180, [0, 0, 1])
      BeltClamp();
  translate([$XPosition + 6, $YPosition + 14,  $FanAdder - 5])
    rotate(180, [0, 0, 1])
      BeltClamp();
  //Belt bearing
//  translate([0, $YPosition + 10.5, 14 + $FanAdder])
  translate([0, $YPosition + 8.5, 32.5 + $FanAdder - 6])
    XPulley($Height = 8.4, $Shaft = 3);
//    cylinder(d = 22, h = 7);

//  //40 mm fan
//  translate([$XPosition - 20, $YPosition - 34,  $FanAdder + 10])
//#  cube([40, 40, 12]);
}

module XCarriageMotorMount()
{
  difference()
  {
    union()
    {
      //Base
      translate([0, 0, -1.5])
        cube([50, 87, 3], center = true);
      //Back support
      translate([-27, 0, 8])
        cube([4, 87, 22], center = true);
      //Side supports
      translate([0, 31, 8])
        cube([50, 4, 22], center = true);
      translate([0, -31, 8])
        cube([50, 4, 22], center = true);
    }
    //Motor opening and mounting holes
    translate([0, 0, .5])
      rotate(90, [1, 0, 0])
        MotorMountHoles();

    $Slot = 5;
    //Holes to mount to Y carriage
    hull()
    {
      translate([$Slot / 2, 0, 0])
        translate([0, 26, 0])
          cylinder(d = 3.2, h = 10, center = true);
      translate([-$Slot / 2, 0, 0])
        translate([0, 26, 0])
          cylinder(d = 3.2, h = 10, center = true);
    }

    hull()
    {
      translate([$Slot / 2, 0, 0])
        translate([0, 36, 0])
          cylinder(d = 3.2, h = 10, center = true);
      translate([-$Slot / 2, 0, 0])
        translate([0, 36, 0])
          cylinder(d = 3.2, h = 10, center = true);
    }

    hull()
    {
      translate([$Slot / 2, 0, 0])
        translate([0, -26, 0])
          cylinder(d = 3.2, h = 10, center = true);
      translate([-$Slot / 2, 0, 0])
        translate([0, -26, 0])
          cylinder(d = 3.2, h = 10, center = true);
    }

    hull()
    {
      translate([$Slot / 2, 0, 0])
        translate([0, -36, 0])
          cylinder(d = 3.2, h = 10, center = true);
      translate([-$Slot / 2, 0, 0])
        translate([0, -36, 0])
          cylinder(d = 3.2, h = 10, center = true);
    }    
    
    //Excess side support meterial
    translate([0, 0, 25])
      rotate(20, [0, 1, 0])
        cube([80, 90, 30], center = true);
  }
}

module YRailSupportBearingEnd()
{
  difference()
  {
    translate([0, 0, $HeightAdjust])
      RodClamp(width = 19, height = $HeightAdjust + 27, screwoffset = 2, depth = 15, boreheight = $HeightAdjust + 20, boreradius = ($Rail6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 1, captivestyle = 1, captivebore =0, captivedepth = 1.5);
  
    //Lock screw holes
    translate([0, 0, $HeightAdjust])
      cylinder(d = $LockScrewBoreM2_5Vertical, h = 20);
  }
  
  //Y screw bearing support
  translate([9, -7.5, -7])
  {
    difference()
    {
      translate([0, 0, -1])
        cube([14, 15, 29]);
      translate([6, 20, 19])
        rotate(90, [1, 0, 0])
          cylinder(d = $YScrewBearingDiameter, h = 25);
      translate([-1, 0, -2])
        rotate(45, [0, 1, 0])
          cube([14, 15, 26]);
    translate([6, 7.5, $HeightAdjust])
      cylinder(d = $LockScrewBoreM2_5Vertical, h = 20);
    }
  }
  
  //Mounting base
  translate([-(32 / 2), -7.5, -26])
  {
    difference()
    {
      translate([-2, 0, 0])
      cube([36, 15, 15]);
      translate([3, 7.5, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 20);
      translate([32 - 3, 7.5, -1])
        cylinder(d = $LockScrewBoreM3Vertical + $VerticalLockClearance, h = 20);
    }
  }
}

module YBacklash()
{
  difference()
  {
    //Base material
    translate([0, -5, -1])
      cube([12, 30, 14], center = true);
    //Threaded shaft opening
    rotate(90, [1, 0, 0])
      cylinder(d = 6.5, h = 45, center = true);
    //Backlash locking nut opening
    translate([0, 6, 0])
    {
      rotate(30, [0, 1, 0])
        rotate(90, [1, 0, 0])
          cylinder(d = (8 + .5) * $RoundToHex, h = 3.4 + .7, center = true, $fn=6);
      translate([0, 0, 5])
        cube([8 + .5, 3.4 + .7, 10], center = true);
    }
    //Backlash captive nut opening
    translate([0, -8, 0])
    {
      rotate(30, [0, 1, 0])
        rotate(90, [1, 0, 0])
          cylinder(d = 9 * $RoundToHex, h = 19, center = true, $fn=6);
      translate([0, 0, 5])
        cube([9, 19, 10], center = true);
    }
  }
}

module DisplayAlignment()
{
  
  translate([0, $YPosition - 100, 14])
    YCarriageCheck();    
    
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

  //This is the dual Y travel motor requirements
    YRailSupportBearingEnd();
    translate([$XRailLength, 0, 0])
      mirror([1, 0, 0])
        YRailSupportBearingEnd();
    translate([0, $YRailLength - 7, 0])
      rotate(180, [0, 0, 1])
        mirror([1, 0, 0])
          YMotorAndShaftMount();
    translate([$XRailLength, $YRailLength - 7, 0])
      rotate(180, [0, 0, 1])
        YMotorAndShaftMount();
      
  translate([0, 0, $HeightAdjust + $FanAdder])
  {

  //This is dual leadscrew
  color([.7, .8, .7])
  {
    translate([15, -5, -22])
      rotate(-90, [1, 0, 0])
        cylinder(d = 5, h = $YRailLength + 3);

    translate([$XRailLength - 15, -5, -22])
      rotate(-90, [1, 0, 0])
        cylinder(d = 5, h = $YRailLength + 3);
  }
  translate([15, $YRailLength + 2, -22])
    rotate(-90, [1, 0, 0])
      Stepper(Width = $XStepperMotorWidth, Depth = $XStepperMotorDepth, Spacing = $XStepperMotorSpacing, $Pulley = 2);

  translate([$XRailLength - 15, $YRailLength + 2, -22])
    rotate(-90, [1, 0, 0])
      Stepper(Width = $XStepperMotorWidth, Depth = $XStepperMotorDepth, Spacing = $XStepperMotorSpacing, $Pulley = 2);

  //Y travel limit switch
  translate([$XRailLength + 9, $YRailLength - 2, -30])
    rotate(90, [0, 1, 0])
      rotate(180, [0, 0, 1])
        LimitSwitch();
    
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

module PrintXSet()
{
  translate([0, 0, 3])
    XCarriageMotorMount();

  translate([50, -8, 10])
      RightSideRailCarriage();

  translate([60, 44, 10])
    rotate(90, [0, 0, 1])
      LeftSideRailCarriage();
  
  FocusTool();
  translate([90, 0, -5])
    XCarriageSquareHost2();

}

module PrintYSet()
{
  YMotorAndShaftMount();
  translate([12, -19, 0])
    YRailSupportBearingEnd();
  translate([50, 0, 0])
    YMotorAndShaftMount();
  translate([62, -19, 0])
    YRailSupportBearingEnd();
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
    cylinder(d = $BearingShaftDiameter, h = 20);
  
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
  translate([670, 0])
    LaserSet3();  
}

module DragChainMount()
{
  difference()
  {
    cube([16, 20, 12.5]);
    translate([1, -1, 1])
      cube([14, 22, 10.5]);
    translate([1.5, -1, 3])
      cube([13, 22, 10.5]);
  }
}

module XCarriageSquareHost2()
{
  difference()
  {
    union()
    {
      translate([0, $CarrierSpacing / 2, $CarrierZOffset])
        rotate(90, [0, 0, 1])
            RodClamp(width = 18, height = 20, screwoffset = 0, depth = 35, boreheight = 9, boreradius = ($Bearing6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 1.5);
      translate([-0, -$CarrierSpacing / 2, $CarrierZOffset])
      {
          rotate(90, [0, 0, 1])
            RodClamp(width = 18, height = 20, screwoffset = 0, depth = 35, boreheight = 9, boreradius = ($Bearing6mmDiameter / 2), gapwidth = 0, screwbore = 0, screwstyle = 0, captivestyle = 0, captivebore = 0, captivedepth = 1.5);
      }
      //Diode mounting block and runner support
      translate([-26, ($CarrierSpacing / 2) -36, 5])
        cube([52, ($CarrierSpacing / 2) + 4.5, 34]);
    }
    

    rotate(90, [0, 0, 1])
    {
      //Belt clamp holes
      translate([16, 22.5, 29])
        BeltClampHolePairVertical($Bore = $LockScrewBoreM2_5Horizontal);
      translate([16, -22.5, 29])
        BeltClampHolePairVertical($Bore = $LockScrewBoreM2_5Horizontal);
    }

    //Screw lock/glue holes
    translate([0, -22.5, 14])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 20);
    translate([0, 22.5, 14])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 20);
    
	  //Diode mounting hole
    translate([0, 2 - 5, 0])
	    cylinder(d1 = $DiodeHousingHole - .2, d2 = $DiodeHousingHole, h = 42);
    translate([-6.5, 2 - (13 / 2) - 5, 32])
    {
      translate([$DiodeHostWidth / 2, $DiodeHostLength / 2, 0])
        linear_extrude(height = 20, scale = 1.1)
          translate([-$DiodeHostWidth / 2, -$DiodeHostLength / 2, 0])
            square([$DiodeHostWidth, $DiodeHostLength]);
    }
    //Remove a little excess plastic
    translate([30, 0, 0])
      rotate(45, [0, 1, 0])
        cube([40, ($CarrierSpacing / 2) + 4.5, 40], center = true);
    translate([-30, 0, 0])
      rotate(45, [0, 1, 0])
        cube([40, ($CarrierSpacing / 2) + 4.5, 40], center = true);
    translate([-22, 8.2, 25])
      cube([20, 10.6, 40], center = true);
    translate([22, 8.2, 25])
      cube([20, 10.6, 40], center = true);
    translate([0, 20, 35])
      cube([50, 20, 20], center = true);
  }
  //Limit switch bumper
  translate([15, -35, 5])
    cube([6, 5, 20]);
}

module FocusTool()
{
  difference()
  {
    union()
    {
      cylinder(d = $DiodeHousingHole - 2, h = 15, $fn = 50);
      cylinder(d = 20, h = 3, $fn = 50);
      translate([0, 0, 15.5])
        cube([.5, 8, 1], center = true);
    }
    cylinder(d2 = 4, d1 = 9,h = 20, $fn = 50);
  }
}

$HorizontalLockClearance = .35;//Delta between horizontal threaded hole and free hole
$LockScrewBoreM2_5Horizontal = 2.3;//3D printed horizontal hole for M2.5 threadlock


module YCoupler1()
{
  $YMotorShaftDiameter = 5.0;//Bore for Y motor attach to coupler
  $YLeadscrewDiameter = 5.0;//Bore for Y leadscrew attach to coupler
  $CoreDiameter = 14;
  $ScrewOffset = 4;
  $HeadOffset = 3.5;
  $HeadSize = 5.5;
  $HorizontalLockClearance = .5;
  
  difference()
  {
    //Core
    rotate(30, [0, 0, 1])
      cylinder(d = $CoreDiameter, h = 20, $fn = 6, center = true);
    //Motor attach opening
    translate([0, 0, 0])
      cylinder(d = $YMotorShaftDiameter, h = 10, $fn = 50);
    //End spring opening
    translate([$CoreDiameter / 2, 0, 0])
      cube([$CoreDiameter, $CoreDiameter, .4], center = true);
    //Leadscrew attach opening
    translate([0, 0, -10])
      cylinder(d = $YLeadscrewDiameter, h = 10, $fn = 50);
    //Clamp gap
    translate([$CoreDiameter / 2, 0, 0])
      cube([$CoreDiameter, 2, 20], center = true);
    //Clamp screw clearance holes
    translate([$ScrewOffset, 0, 5])
      rotate(90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal + $HorizontalLockClearance, h = 15, $fn = 50);
    translate([$ScrewOffset, 0, -5])
      rotate(90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal + $HorizontalLockClearance, h = 15, $fn = 50);
    //Clamp screw grip holes
    translate([$ScrewOffset, 0, 5])
      rotate(-90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 15, $fn = 50);
    translate([$ScrewOffset, 0, -5])
      rotate(-90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 15, $fn = 50);
    //Clamp screw head openings
    translate([$ScrewOffset, -$HeadOffset, 5])
      rotate(90, [1, 0, 0])
        cylinder(d = $HeadSize, h = 15, $fn = 50);
    translate([$ScrewOffset, -$HeadOffset, -5])
      rotate(90, [1, 0, 0])
        cylinder(d = $HeadSize, h = 15, $fn = 50);
  }
}

module YCoupler2()
{
  $YMotorShaftDiameter = 5.1;//Bore for Y motor attach to coupler
  $YLeadscrewDiameter = 5.1;//Bore for Y leadscrew attach to coupler
  $CoreDiameter = $YMotorShaftDiameter + 2;
  $ScrewOffset = 4.5;
  $HeadSize = 5.5;
  $NutSize = 5.5;
  $HorizontalLockClearance = .5;
  $ClampWidth = 4;
  $HeadOffset = $ClampWidth / 2;
  
  difference()
  {
    //Core
      union()
      {
        cylinder(d = $CoreDiameter, h = 20, $fn = 100, center = true);
        translate([4.5, 0, 0])
          cube([5, $ClampWidth, 20], center = true);
      }
    //Motor attach opening
    translate([0, 0, 0])
      cylinder(d = $YMotorShaftDiameter, h = 10, $fn = 50);
    //End spring openings
    translate([($CoreDiameter / 2) + ($YLeadscrewDiameter / 3), 0, 0])
      cube([$CoreDiameter, $CoreDiameter, .4], center = true);
    translate([-($CoreDiameter / 2) - ($YLeadscrewDiameter / 3), 0, 0])
      cube([$CoreDiameter, $CoreDiameter, .4], center = true);
    //Leadscrew attach opening
    translate([0, 0, -10])
      cylinder(d = $YLeadscrewDiameter, h = 10, $fn = 50);
    //Clamp gap
    translate([$CoreDiameter / 2, 0, 0])
      cube([$CoreDiameter, 1, 20], center = true);
    //Clamp screw clearance holes
    translate([$ScrewOffset, 0, 5])
      rotate(90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal + $HorizontalLockClearance, h = 15, $fn = 50);
    translate([$ScrewOffset, 0, -5])
      rotate(90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal + $HorizontalLockClearance, h = 15, $fn = 50);
    //Clamp screw grip holes
    translate([$ScrewOffset, 0, 5])
      rotate(-90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 15, $fn = 50);
    translate([$ScrewOffset, 0, -5])
      rotate(-90, [1, 0, 0])
        cylinder(d = $LockScrewBoreM2_5Horizontal, h = 15, $fn = 50);
    //Clamp screw head openings
    translate([$ScrewOffset, -$HeadOffset, 5])
      rotate(90, [1, 0, 0])
        cylinder(d = $HeadSize, h = 15, $fn = 50);
    translate([$ScrewOffset, -$HeadOffset, -5])
      rotate(90, [1, 0, 0])
        cylinder(d = $HeadSize, h = 15, $fn = 50);
    //Clamp nut head openings
    translate([$ScrewOffset, $HeadOffset, 5])
      rotate(-90, [1, 0, 0])
        rotate(30, [0, 0, 1])
          cylinder(d = $HeadSize, h = 15, $fn = 6);
    translate([$ScrewOffset, $HeadOffset, -5])
      rotate(-90, [1, 0, 0])
        rotate(30, [0, 0, 1])
          cylinder(d = $HeadSize, h = 15, $fn = 6);
  }
}

/////////////////////////////////////////////////////////////////////////

//DisplayAlignment();
//AlignmentBars();
//DragChainMount();

//////////////////////////////////////////////////
//PrintXSet();
//PrintYSet();
//BasePlate();
//YCarriageCheck();
//CalibrationPrint();
//YMotorAndShaftMount();
//YRailSupportBearingEnd();
//MotorMountBlock();
YCoupler2();