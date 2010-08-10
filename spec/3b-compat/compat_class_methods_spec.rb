require File.expand_path('../../spec_helper', __FILE__)

describe "Date/DateTime class methods" do
  it "#new0 should be the same as new!" do
    Date.new0(2454156).should == Date.jd(2454156)
  end

  it "#new1 should be the same as jd" do
    Date.new1(2454156).should == Date.jd(2454156)
  end

  it "#new2 should be the same as ordinal" do
    Date.new2(2008, 10).should == Date.ordinal(2008, 10)
  end

  it "#new3 should be the same as new" do
    Date.new3(2008, 10, 2).should == Date.new(2008, 10, 2)
  end

  it "#neww should be the same as commercial" do
    Date.neww(2008, 10, 2).should == Date.commercial(2008, 10, 2)
  end

  it "#_parse should parse the date string and return a hash" do
    Date._parse('2008-10-20 11:12:13.0-08:00').should == {:mon=>10, :zone=>'-08:00', :sec=>13, :sec_fraction=>0.0, :year=>2008, :hour=>11, :offset=>-28800, :mday=>20, :min=>12}
    Date._parse('2008-10-20 11:12:13.0-08:00', true).should == {:mon=>10, :zone=>'-08:00', :sec=>13, :sec_fraction=>0.0, :year=>2008, :hour=>11, :offset=>-28800, :mday=>20, :min=>12}
  end

  it "#_parse should not contain fields in the hash that were guessed" do
    Date._parse('2008-10-20').should == {:year=>2008, :mday=>20, :mon=>10}
    Date._parse('11:12:13').should == {:sec=>13, :hour=>11, :min=>12}
  end
  
  it "#ajd_to_amjd should convert a astronomical julian date to a astronomical modified julian date" do
    Date.ajd_to_amjd(2400002).should == 1
  end

  it "#ajd_to_jd should convert a astronomical julian date to a julian date" do
    Date.ajd_to_jd(1).should == [1, 0.5]
    Date.ajd_to_jd(1, 1).should == [1, 0.5]
  end

  it "#amjd_to_amd should convert a astronomical modified julian date to a astronomical julian date" do
    Date.amjd_to_ajd(1).should == 2400001
  end

  it "#civil_to_jd should convert a civil date into a Julian day number" do
    Date.civil_to_jd(2007, 2, 24).should == 2454156
    Date.civil_to_jd(2007, 2, 24, 1).should == 2454156
  end

  it "#commercial_to_jd should convert a commercial date (year - week - day of week) into a Julian day number" do
    Date.commercial_to_jd(2007, 45, 1).should == 2454410
    Date.commercial_to_jd(2007, 45, 1, 1).should == 2454410
  end

  it "#day_fraction_to_time should convert a fraction of a day to an array of time parts" do
    Date.day_fraction_to_time(0.5).should == [12, 0, 0, 0.0]
  end

  it "#gregorian? should return whether the first argument is greater than or equal the second" do
    Date.gregorian?(1, 2).should == false
    Date.gregorian?(2, 1).should == true
    Date.gregorian?(1, 1).should == true
  end

  it "#ns? should be the same as gregorian?" do
    Date.ns?(2, 1).should == Date.gregorian?(2, 1)
    Date.ns?(1, 2).should == Date.gregorian?(1, 2)
    Date.ns?(1, 1).should == Date.gregorian?(1, 1)
  end

  it "#leap? should determine whether a year is a leap year" do
    Date.leap?(1900).should == false
    Date.leap?(1901).should == false
    Date.leap?(1899).should == false
    Date.leap?(1904).should == true
    Date.leap?(1896).should == true
    Date.leap?(1999).should == false
    Date.leap?(2001).should == false
    Date.leap?(2000).should == true
  end

  it "#gregorian_leap? should be the same as leap?" do
    Date.gregorian_leap?(2000).should == Date.leap?(2000)
  end

  it "#jd_to_ajd should convert a Julian day into a astronomical julian date" do
    Date.jd_to_ajd(2454156, 0).should == 2454156
    Date.jd_to_ajd(2454156, 1, 1).should == 2454156
  end

  it "#jd_to_civil should convert a Julian day into a civil date" do
    Date.jd_to_civil(2454156).should == [2007, 2, 24]
    Date.jd_to_civil(2454156, 1).should == [2007, 2, 24]
  end

  it "#jd_to_commercial should convert a Julian day number into a commercial date" do
    Date.jd_to_commercial(2454410).should == [2007, 45, 1]
    Date.jd_to_commercial(2454410, 1).should == [2007, 45, 1]
  end

  it "#jd_to_ld should convert a Julian day into a number of days since italian calendar reform day" do
    Date.jd_to_ld(2299161).should == 1
  end

  it "#jd_to_mjd should convert a Julian day into a modified julian date" do
    Date.jd_to_mjd(2400002).should == 1
  end

  it "#jd_to_ordinal should convert a Julian day number into an ordinal date" do
    Date.jd_to_ordinal(2454156).should == [2007, 55]
    Date.jd_to_ordinal(2454156, 1).should == [2007, 55]
  end

  it "#jd_to_wday should be able to convert a Julian day number into a week day number" do
    Date.jd_to_wday(2454482).should == 3
  end

  it "#julian? should return whether the first argument is less than the second" do
    Date.julian?(1, 1).should == false
    Date.julian?(1, 2).should == true
  end

  it "#os? should be the same as julian?" do
    Date.os?(1, 2).should == Date.julian?(1, 2)
  end

  it "#julian leap? should determine whether a year is a leap year in the julian calendar" do
    Date.julian_leap?(1900).should == true
    Date.julian_leap?(1901).should == false
    Date.julian_leap?(1899).should == false
    Date.julian_leap?(1904).should == true
    Date.julian_leap?(1896).should == true
    Date.julian_leap?(1999).should == false
    Date.julian_leap?(2001).should == false
    Date.julian_leap?(2000).should == true
  end

  it "#ld_to_jd should convert a number of days since italian calendar reform day to a julian date" do
    Date.ld_to_jd(1).should == 2299161
  end

  it "#mjd_to_jd should convert a modified julian date into a julian date" do
    Date.mjd_to_jd(1).should == 2400002
  end

  it "#ordinal_to_jd should convert an ordinal date (year-day) to a Julian day number" do
    Date.ordinal_to_jd(2007, 55).should == 2454156
  end

  it "#time_to_day_fraction should convert time parts to a fraction" do
    Date.time_to_day_fraction(12, 0, 0).should == 0.5
  end

  it "#valid_civil? should return corresponding julian date if valid and nil if not" do
    Date.valid_civil?(2007, 2, 24).should == 2454156
    Date.valid_civil?(2007, 2, 24, 1).should == 2454156
    Date.valid_civil?(2007, 2, 30, 1).should == nil
  end

  it "#valid_date? should be the same as valid_civil?" do
    Date.valid_date?(2007, 2, 24, 1).should == Date.valid_civil?(2007, 2, 24, 1)
  end

  it "#exist? should be the same as valid_date?" do
    Date.exist?(2007, 2, 24, 1).should == Date.valid_date?(2007, 2, 24, 1)
  end

  it "#exist3? should be the same as valid_date?" do
    Date.exist3?(2007, 2, 24, 1).should == Date.valid_date?(2007, 2, 24, 1)
  end

  it "#valid_commercial? should return corresponding julian date if valid and nil if not" do
    Date.valid_commercial?(2007, 45, 1).should == 2454410
    Date.valid_commercial?(2007, 45, 1, 1).should == 2454410
    Date.valid_commercial?(2007, 54, 1, 1).should == nil
  end

  it "#existw? should be the same as valid_date?" do
    Date.existw?(2007, 45, 1, 1).should == Date.valid_commercial?(2007, 45, 1, 1)
  end

  it "#valid_jd? should return the given julian date" do
    Date.valid_jd?(2454156).should == 2454156
    Date.valid_jd?(2454156, 1).should == 2454156
  end

  it "#exist1? should be the same as valid_jd?" do
    Date.exist1?(2454156, 1).should == Date.valid_jd?(2454156, 1)
  end

  it "#valid_ordinal? should return corresponding julian date if valid and nil if not" do
    Date.valid_ordinal?(2007, 55).should == 2454156
    Date.valid_ordinal?(2007, 55, 1).should == 2454156
    Date.valid_ordinal?(2007, 367, 1).should == nil
  end

  it "#exist2? should be the same as valid_ordinal?" do
    Date.exist2?(2007, 55, 1).should == Date.valid_ordinal?(2007, 55)
  end

  it "#valid_time? should return corresponding day fraction if valid and nil if not" do
    Date.valid_time?(12, 0, 0).should == 0.5
    Date.valid_time?(25, 0, 0).should == nil
    Date.valid_time?(12, 61, 0).should == nil
    Date.valid_time?(12, 0, 61).should == nil
  end

  it "#zone_to_diff? should return the offset in seconds if a valid time zone, or 0 if not" do
    Date.zone_to_diff('-0800').should == -28800
    Date.zone_to_diff('+08:00').should == 28800
    Date.zone_to_diff('YY').should == 0
  end

end
