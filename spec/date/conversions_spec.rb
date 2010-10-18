require File.expand_path('../../spec_helper', __FILE__)

describe "Date#new_start" do
  it "should convert a date object into another with a new calendar reform" do
    Date.civil(1582, 10, 14, Date::ENGLAND).new_start.should == Date.civil(1582, 10, 14)
    Date.civil(1582, 10,  4, Date::ENGLAND).new_start.should == Date.civil(1582, 10,  4)
    Date.civil(1582, 10, 15).new_start(Date::ENGLAND).should == Date.civil(1582, 10, 15, Date::ENGLAND)
    Date.civil(1752,  9, 14).new_start(Date::ENGLAND).should == Date.civil(1752,  9, 14, Date::ENGLAND)
    Date.civil(1752,  9, 13).new_start(Date::ENGLAND).should == Date.civil(1752,  9, 13, Date::ENGLAND)
  end
  
  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.jd.new_start.should be_kind_of(c)
  end

  ruby_version_is "" ... "1.9" do
    it "#newsg should be the same as new_start" do
      Date.civil(1582, 10, 14, Date::ENGLAND).newsg.should == Date.civil(1582, 10, 14, Date::ENGLAND).new_start
      Date.civil(1582, 10, 14, Date::ENGLAND).newsg(1).should == Date.civil(1582, 10, 14, Date::ENGLAND).new_start(1)
    end
  end
end

describe "Date#italy" do
  it "should convert a date object into another with the Italian calendar reform" do
    Date.civil(1582, 10, 14, Date::ENGLAND).italy.should == Date.civil(1582, 10, 14)
    Date.civil(1582, 10,  4, Date::ENGLAND).italy.should == Date.civil(1582, 10,  4)
  end
end

describe "Date#england" do
  it "should convert a date object into another with the English calendar reform" do
    Date.civil(1582, 10, 15).england.should == Date.civil(1582, 10, 15, Date::ENGLAND)
    Date.civil(1752,  9, 14).england.should == Date.civil(1752,  9, 14, Date::ENGLAND)
    Date.civil(1752,  9, 13).england.should == Date.civil(1752,  9, 13, Date::ENGLAND)
  end
end

describe "Date#julian" do
  it "should convert a date object into another with the Julian calendar" do
    Date.civil(1582, 10, 15).julian.should == Date.civil(1582, 10,  15, Date::JULIAN)
    Date.civil(1752,  9, 14).julian.should == Date.civil(1752,  9,  14, Date::JULIAN)
    Date.civil(1752,  9, 13).julian.should == Date.civil(1752,  9,  13, Date::JULIAN)
  end
end

describe "Date#gregorian" do
  it "should convert a date object into another with the Gregorian calendar" do
    Date.civil(1582, 10, 14).gregorian.should == Date.civil(1582, 10, 14, Date::GREGORIAN)
    Date.civil(1752,  9, 14).gregorian.should == Date.civil(1752,  9, 14, Date::GREGORIAN)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.jd.gregorian.should be_kind_of(c)
  end
end

ruby_version_is "" ... "1.9" do
  describe "Date#ordinal_to_jd" do
    it "should convert an ordinal date (year-day) to a Julian day number" do
      Date.ordinal_to_jd(2007, 55).should == 2454156
    end
  end
  
  describe "Date#jd_to_ordinal" do
    it "should convert a Julian day number into an ordinal date" do
      Date.jd_to_ordinal(2454156).should == [2007, 55]
      Date.jd_to_ordinal(2454156, 1).should == [2007, 55]
    end
  end
  
  describe "Date#civil_to_jd" do
    it "should convert a civil date into a Julian day number" do
      Date.civil_to_jd(2007, 2, 24).should == 2454156
      Date.civil_to_jd(2007, 2, 24, 1).should == 2454156
    end
  end
  
  describe "Date#jd_to_civil" do
    it "should convert a Julian day into a civil date" do
      Date.jd_to_civil(2454156).should == [2007, 2, 24]
      Date.jd_to_civil(2454156, 1).should == [2007, 2, 24]
    end
  end
  
  describe "Date#commercial_to_jd" do
    it "should convert a commercial date (year - week - day of week) into a Julian day number" do
      Date.commercial_to_jd(2007, 45, 1).should == 2454410
      Date.commercial_to_jd(2007, 45, 1, 1).should == 2454410
    end
  end
  
  describe "Date#jd_to_commercial" do
    it "should convert a Julian day number into a commercial date" do
      Date.jd_to_commercial(2454410).should == [2007, 45, 1]
      Date.jd_to_commercial(2454410, 1).should == [2007, 45, 1]
    end
  end
  
  describe "Date#ajd_to_jd" do
    it "should convert a Astronomical Julian day number into a Julian day number" do
      Date.ajd_to_jd(2454410).should == [2454410, 0.5]
      Date.ajd_to_jd(2454410, 1.0/ 2).should == [2454410, 0.5]
      Date.ajd_to_jd(1).should == [1, 0.5]
      Date.ajd_to_jd(1, 1).should == [1, 0.5]
    end
  end
  
  describe "Date#jd_to_ajd" do
    it "should convert a Julian day number into a Astronomical Julian day number" do
      Date.jd_to_ajd(2454410, 0).should == 2454410
      Date.jd_to_ajd(2454410, 1.0/ 2).should == 2454410
      Date.jd_to_ajd(2454156, 0).should == 2454156
      Date.jd_to_ajd(2454156, 1, 1).should == 2454156
    end
  end
  
  describe "Date#day_fraction_to_time" do
    it "should be able to convert a day fraction into time" do
      Date.day_fraction_to_time(2).should == [48, 0, 0, 0]
      Date.day_fraction_to_time(1).should == [24, 0, 0, 0]
      Date.day_fraction_to_time(1.0/ 2).should == [12, 0, 0, 0]
      a = Date.day_fraction_to_time(1.0/ 7)
      a.pop.should be_close(1.0/ 100800, 0.00003)
      a.should == [3, 25, 42]
    end
  end
  
  describe "Date#time_to_day_fraction" do
    it "should be able to convert a time into a day fraction" do
      Date.time_to_day_fraction(48, 0, 0).should == 2.0
      Date.time_to_day_fraction(24, 0, 0).should == 1.0
      Date.time_to_day_fraction(12, 0, 0).should == 0.5
      Date.time_to_day_fraction(10, 20, 10).should == 10.0/24.0 + 20.0/(24 * 60) + 10.0/(24 * 60 * 60)
    end
  end
  
  describe "Date#amjd_to_ajd" do
    it "shoud be able to convert Astronomical Modified Julian day numbers into Astronomical Julian day numbers" do
      Date.amjd_to_ajd(10).should == 2400010
    end
  end
  
  describe "Date#ajd_to_amjd" do
    it "shoud be able to convert Astronomical Julian day numbers into Astronomical Modified Julian day numbers" do
      Date.ajd_to_amjd(10000010).should == 7600009
    end
  end
  
  describe "Date#mjd_to_jd" do
    it "shoud be able to convert Modified Julian day numbers into Julian day numbers" do
      Date.mjd_to_jd(2000).should == 2000 + 2400001
    end
  end
  
  describe "Date#jd_to_mjd" do
    it "shoud be able to convert Julian day numbers into Modified Julian day numbers" do
      Date.jd_to_mjd(2500000).should == 2500000 - 2400001
    end
  end
  
  describe "Date#ld_to_jd" do
    it "should be able to convert the number of days since the Gregorian calendar in Italy into Julian day numbers" do
      Date.ld_to_jd(450000).should == 450000 + 2299160
    end
  end
  
  describe "Date#jd_to_ld" do
    it "should be able to convert Julian day numbers into the number of days since the Gregorian calendar in Italy" do
      Date.jd_to_ld(2450000).should == 2450000 - 2299160
    end
  end
  
  describe "Date#jd_to_wday" do
    it "should be able to convert a Julian day number into a week day number" do
      Date.jd_to_wday(2454482).should == 3
    end
  end
end

describe "Date instance methods" do
  before do
    @d = Date.civil(2008, 10, 11)
  end

  it "#ajd should be the same as jd" do
    @d.ajd.should == @d.jd
  end

  it "#amjd should be the astronomical modified julian date" do
    @d.amjd.should == 54750
  end

  it "#ld should be the days since italian calendar reform day" do
    @d.ld.should == 155591
  end

  it "#mjd should be the modified julian date" do
    @d.mjd.should == 54750
  end
end

ruby_version_is "" ... "1.9" do
  describe "Date.valid_time?" do
    it " should return corresponding day fraction if valid and nil if not" do
      Date.valid_time?(12, 0, 0).should == 0.5
      Date.valid_time?(25, 0, 0).should == nil
      Date.valid_time?(12, 61, 0).should == nil
      Date.valid_time?(12, 0, 61).should == nil
    end
  end
end

describe "Date marshalling" do
  it "should marshal and unmarshal correctly" do
    Marshal.load(Marshal.dump(Date.jd)).should == Date.civil
    Marshal.load(Marshal.dump(Date.today)).should == Date.today
    Marshal.load(Marshal.dump(Date.civil(2010, 2, 4))).should == Date.civil(2010, 2, 4)
  end
end

ruby_version_is "1.9" do
  describe "Date#to_date" do
    it " should return the receiver" do
      Date.new(2009, 1, 2).to_date.should == Date.new(2009, 1, 2)
    end
  end

  describe "Date#to_datetime" do
    it " should return a DateTime equal to the receiver" do
      Date.new(2009, 1, 2).to_datetime.should == DateTime.new(2009, 1, 2)
    end
  end

  describe "Date#to_time" do
    it " should return a Time in local time with the same year, month, day as the receiver" do
      Date.new(2009, 1, 2).to_time.should == Time.local(2009, 1, 2)
    end
  end

  describe "Time#to_date" do
    it " should return a Date with the same year, month, and day as the receiver" do
      Time.local(2009, 1, 2).to_date.should == Date.new(2009, 1, 2)
    end
  end

  describe "Time#to_time" do
    it " should return the receiver in local time" do
      Time.local(2009, 1, 2).to_time.should == Time.local(2009, 1, 2)
      Time.local(2009, 1, 2).getutc.to_time.should == Time.local(2009, 1, 2)
    end
  end
end

