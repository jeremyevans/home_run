require File.expand_path('../../spec_helper', __FILE__)

describe "Date#strptime" do

  it "._strptime should not accept less than 1 arguments" do
    proc{Date._strptime}.should raise_error(ArgumentError)
  end

  it "._strptime should not accept more than 2 arguments" do
    proc{Date._strptime('2008-10-11', '%Y-%m-%d', '1')}.should raise_error(ArgumentError)
  end

  it "._strptime should be a hash of values" do
    Date._strptime('2008-10-11').should == {:year=>2008, :mon=>10, :mday=>11}
    Date._strptime('2008-10-11', '%Y-%m-%d').should == {:year=>2008, :mon=>10, :mday=>11}
  end
  
  it "should have defaults and an optional sg value" do
    Date.strptime.should == Date.jd
    Date.strptime('2008-10-11').should == Date.civil(2008, 10, 11)
    Date.strptime('2008-10-11', '%Y-%m-%d').should == Date.civil(2008, 10, 11)
    Date.strptime('2008-10-11', '%Y-%m-%d', 1).should == Date.civil(2008, 10, 11)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.strptime.should be_kind_of(c)
    c.strptime('20081011', '%Y%m%d').should be_kind_of(c)
  end

  it "raises errors for invalid dates" do
    lambda { Date.strptime("") }.should raise_error(ArgumentError)
    lambda { Date.strptime("", "") }.should raise_error(ArgumentError)
    lambda { Date.strptime("2009-02-29") }.should raise_error(ArgumentError)
  end

  it "uses the default format when not given a date format" do
    Date.strptime("2000-04-06").should == Date.civil(2000, 4, 6)
    Date.civil(2000, 4, 6).strftime.should == Date.civil(2000, 4, 6).to_s
  end

  it "parses a full day name" do
    d = Date.today
    expected_date = Date.commercial(d.cwyear, d.cweek, 4)
    # strptime assumed week that start on sunday, not monday
    expected_date += 7 if d.cwday == 7
    Date.strptime("Thursday", "%A").should == expected_date
  end

  it "parses a short day name" do
    d = Date.today
    expected_date = Date.commercial(d.cwyear, d.cweek, 4)
    # strptime assumed week that start on sunday, not monday
    expected_date += 7 if d.cwday == 7
    Date.strptime("Thu", "%a").should == expected_date
  end

  it "parses only the short part of the day name" do
    Date._strptime("Friday", "%aday").should == {:wday=>5}
  end

  it "parses a full month name" do
    d = Date.today
    Date.strptime("April", "%B").should == Date.civil(d.year, 4, 1)
  end

  it "parses a short month name" do
    d = Date.today
    Date.strptime("Apr", "%b").should == Date.civil(d.year, 4, 1)
    Date.strptime("Apr", "%h").should == Date.civil(d.year, 4, 1)
  end

  it "parses only the short part of the month name" do
    d = Date.today
    Date.strptime("April", "%bil").should == Date.civil(d.year, 4, 1)
    Date.strptime("April", "%hil").should == Date.civil(d.year, 4, 1)
  end

  it "parses a century" do
    Date.strptime("06 20", "%y %C").should == Date.civil(2006, 1, 1)
  end

  it "parses a month day with leading zeroes" do
    d = Date.today
    Date.strptime("06", "%d").should == Date.civil(d.year, d.month, 6)
  end

  it "parses a month day with leading spaces" do
    d = Date.today
    Date.strptime(" 6", "%e").should == Date.civil(d.year, d.month, 6)
  end

  it "parses a commercial year" do
    Date.strptime("2000", "%G").should == Date.civil(2000,  1,  3)
    Date.strptime("2002", "%G").should == Date.civil(2001, 12, 31)
    Date.strptime("20000", "%G").should == Date.civil(20000,  1,  3)
  end

  it "parses a commercial year with trailing numbers" do
    Date.strptime("20000", "%G0").should == Date.civil(2000,  1,  3)
    Date.strptime("20023", "%G%u").should == Date.civil(2002, 1, 2)
  end

  it "parses a commercial year with only two digits" do
    Date.strptime("68", "%g").should == Date.civil(2068,  1,  2)
    Date.strptime("69", "%g").should == Date.civil(1968, 12, 30)
  end

  it "parses a year day with leading zeroes" do
    d = Date.today
    if Date.gregorian_leap?(Date.today.year)
      Date.strptime("097", "%j").should == Date.civil(d.year, 4, 6)
    else
      Date.strptime("097", "%j").should == Date.civil(d.year, 4, 7)
    end
  end

  it "parses a month with leading zeroes" do
    d = Date.today
    Date.strptime("04", "%m").should == Date.civil(d.year, 4, 1)
  end
  
  it "should be able to parse the number of seconds since the unix epoch" do
    Date.strptime("954979200000", "%Q").should == Date.civil(2000, 4, 6)
    Date.strptime("32511888000000", "%Q").should == Date.civil(3000, 4, 6)
  end
  
  it "should be able to parse the number of seconds since the unix epoch" do
    Date.strptime("954979200", "%s").should == Date.civil(2000, 4, 6)
    Date.strptime("32511888000", "%s").should == Date.civil(3000, 4, 6)
  end
  
  it "should be able to pase the commercial day" do
    Date.strptime("1", "%u").should == Date.commercial(Date.today.year, 1, 1)
    Date.strptime("15 3", "%V %u").should == Date.commercial(Date.today.year, 15, 3)
  end
  
  it "should be able to show the commercial week" do
    d = Date.commercial(Date.today.year,1,1)
    Date.strptime("1", "%V").should == d
    Date.strptime("15", "%V").should == Date.commercial(d.cwyear, 15, 1)
  end

  it "parses a week number for a week starting on Sunday" do
    Date.strptime("2010/1", "%Y/%U").should == Date.civil(2010, 1, 3)
  end

  it "parses a week number for a week starting on Monday" do
    Date.strptime("2010/1", "%Y/%W").should == Date.civil(2010, 1, 4)
  end

  it "parses a commercial week day" do
    Date.strptime("2008 1", "%G %u").should == Date.civil(2007, 12, 31)
  end

  it "parses a commercial week" do
    d = Date.commercial(Date.today.cwyear,1,1)
    Date.strptime("1", "%V").should == d
    Date.strptime("15", "%V").should == Date.commercial(d.cwyear, 15, 1)
  end

  it "parses a week day" do
    d = Date.today
    Date.strptime("2007 4", "%Y %w").should == Date.civil(2007, 1, 4)
  end

  it "parses a full year " do
    Date.strptime("2007", "%Y").should == Date.civil(2007, 1, 1)
    Date.strptime("200", "%Y").should == Date.civil(200, 1, 1)
    Date.strptime("20000", "%Y").should == Date.civil(20000, 1, 1)
  end

  it "parses a full year with trailing numbers" do
    Date.strptime("20000", "%Y0").should == Date.civil(2000, 1, 1)
    Date.strptime("200002", "%Y%m").should == Date.civil(2000, 2, 1)
  end

  it "parses a year in YY format" do
    Date.strptime("00", "%y").should == Date.civil(2000, 1, 1)
  end
  
  it "should be able to parse escapes" do
    Date.strptime("00 % \n \t %1", "%y %% %n %t %%1").should == Date.civil(2000, 1, 1)
  end

  it "parse a %Y%m%d date" do
    Date.strptime("20000203", "%Y%m%d").should == Date.civil(2000, 2, 3)
  end
  

  ############################
  # Specs that combine stuff #
  ############################

  it "parses a full date" do
    Date.strptime("Thu Apr  6 00:00:00 2000", "%c").should == Date.civil(2000, 4, 6)
    Date.strptime("Thu Apr  6 00:00:00 2000", "%a %b %e %H:%M:%S %Y").should == Date.civil(2000, 4, 6)
  end

  it "parses a full date with trailing digits" do
    Date.strptime("Thu Apr  6 00:00:00 20000", "%c0").should == Date.civil(2000, 4, 6)
    Date.strptime("Thu Apr  6 00:00:00 200012", "%c%m").should == Date.civil(2000, 12, 6)
  end

  it "parses a date with slashes" do
    Date.strptime("04/06/00", "%D").should == Date.civil(2000, 4, 6)
    Date.strptime("04/06/00", "%m/%d/%y").should == Date.civil(2000, 4, 6)
  end

  it "parses a date given as YYYY-MM-DD" do
    Date.strptime("2000-04-06", "%F").should == Date.civil(2000, 4, 6)
    Date.strptime("2000-04-06", "%Y-%m-%d").should == Date.civil(2000, 4, 6)
  end

  it "parses a civil format with month name" do
    Date.strptime(" 9-Apr-2000", "%v").should == Date.civil(2000, 4, 9)
    Date.strptime(" 9-Apr-2000", "%e-%b-%Y").should == Date.civil(2000, 4, 9)
  end
  
  it "parses a civil format with month name with trailing digits" do
    Date.strptime(" 9-Apr-20000", "%v0").should == Date.civil(2000, 4, 9)
    Date.strptime(" 9-Apr-200012", "%v%m").should == Date.civil(2000, 12, 9)
  end
  
  it "parses a date given MM/DD/YY" do
    Date.strptime("04/06/00", "%x").should == Date.civil(2000, 4, 6)
    Date.strptime("04/06/00", "%m/%d/%y").should == Date.civil(2000, 4, 6)
  end
  
  it "parses a date given in full notation" do
    Date.strptime("Sun Apr  9 00:00:00 +00:00 2000", "%+").should == Date.civil(2000, 4, 9)
    Date.strptime("Sun Apr  9 00:00:00 +00:00 2000", "%a %b %e %H:%M:%S %Z %Y").should == Date.civil(2000, 4, 9)
  end

  it "parses a date given in full notation with trailing digits" do
    Date.strptime("Sun Apr  9 00:00:00 +00:00 20000", "%+0").should == Date.civil(2000, 4, 9)
    Date.strptime("Sun Apr  9 00:00:00 +00:00 200012", "%+%m").should == Date.civil(2000, 12, 9)
  end
end
