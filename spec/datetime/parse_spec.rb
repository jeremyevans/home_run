require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime._parse" do
  it "should return a hash of values" do
    DateTime._parse("01:02:03").should == {:hour=>1, :min=>2, :sec=>3}
  end

  it "should have :sec_fraction entry be the fraction of second" do
    DateTime._parse('12:13:15.678900')[:sec_fraction].should be_close(0.6789, 0.0000000000001)
  end 
  
  it "should parse zone format returned by postgresql" do
    DateTime._parse('2001-01-01 01:01:01+03').should == {:year=>2001, :mon=>1, :mday=>1, :hour=>1, :min=>1, :sec=>1, :zone=>'+03', :offset=>10800}
  end
  
  it "should parse microseconds and zone by postgresql format" do
    hash = DateTime._parse('2001-01-01 01:01:01.00001-03')
    hash[:sec_fraction].should be_close(0.00001, 1e-14)
    hash[:zone].should == '-03'
    hash[:offset].should == -10800
  end
  
  it "should parse zone format as returned by sqlite3" do
    DateTime._parse('2001-01-01 01:01:01+0300').should == {:year=>2001, :mon=>1, :mday=>1, :hour=>1, :min=>1, :sec=>1, :zone=>'+0300', :offset=>10800}
  end
end

describe "DateTime.parse" do
  it "should have defaults and an optional sg value" do
    DateTime.parse('2008-10-11').should == DateTime.civil(2008, 10, 11)
    DateTime.parse('2008-10-11', true).should == DateTime.civil(2008, 10, 11)
    DateTime.parse('2008-10-11', true, 1).should == DateTime.civil(2008, 10, 11)
    DateTime.parse.should == DateTime.jd
  end
  
  it "should keep the same class as the receiver" do
    c = Class.new(DateTime)
    c.parse.should be_kind_of(c)
    c.parse('2008-10-11').should be_kind_of(c)
  end

  it "can't handle a empty string" do
    lambda{ DateTime.parse("") }.should raise_error(ArgumentError)
  end

  # Specs using numbers
  it "can't handle a single digit" do
    lambda{ DateTime.parse("1") }.should raise_error(ArgumentError)
  end

  it "can handle many different types of time values" do
    DateTime.parse("01:02:03").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 1, 2, 3)
    DateTime.parse("01:02:03am").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 1, 2, 3)
    DateTime.parse(" 1:02:03am").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 1, 2, 3)
    DateTime.parse("1:02:03am").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 1, 2, 3)
    DateTime.parse("01:02:03am").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 1, 2, 3)
    DateTime.parse("01:02:03pm").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 13, 2, 3)
    DateTime.parse("01:02:03pm").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 13, 2, 3)
    DateTime.parse("12:02:03am").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 0, 2, 3)
    DateTime.parse("12:02:03pm").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 12, 2, 3)
  end

  it "should assume no time offset if no time offset is specified" do
    DateTime.parse("01:02:03").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 1, 2, 3, 0)
    DateTime.parse("01:02:03Z").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 1, 2, 3, 0)
    DateTime.parse("01:02:03+0100").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 1, 2, 3, 1/24.0)
    DateTime.parse("01:02:03-01:00").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 1, 2, 3, -1/24.0)
    DateTime.parse("01:02:03+01").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 1, 2, 3, 1/24.0)
    DateTime.parse("01:02:03-01").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 1, 2, 3, -1/24.0)
  end

  it "should parse the time zone abbreviations supported by ruby's Time class" do
    DateTime.parse("01:02:03 UTC").offset.should == 0
    DateTime.parse("01:02:03 UT").offset.should == 0
    DateTime.parse("01:02:03 GMT").offset.should == 0
    DateTime.parse("01:02:03 EST").offset.should == -5/24.0
    DateTime.parse("01:02:03 EDT").offset.should == -4/24.0
    DateTime.parse("01:02:03 CST").offset.should == -6/24.0
    DateTime.parse("01:02:03 CDT").offset.should == -5/24.0
    DateTime.parse("01:02:03 MST").offset.should == -7/24.0
    DateTime.parse("01:02:03 MDT").offset.should == -6/24.0
    DateTime.parse("01:02:03 PST").offset.should == -8/24.0
    DateTime.parse("01:02:03 PDT").offset.should == -7/24.0
    DateTime.parse("01:02:03 A").offset.should == 1/24.0
    DateTime.parse("01:02:03 B").offset.should == 2/24.0
    DateTime.parse("01:02:03 C").offset.should == 3/24.0
    DateTime.parse("01:02:03 D").offset.should == 4/24.0
    DateTime.parse("01:02:03 E").offset.should == 5/24.0
    DateTime.parse("01:02:03 F").offset.should == 6/24.0
    DateTime.parse("01:02:03 G").offset.should == 7/24.0
    DateTime.parse("01:02:03 H").offset.should == 8/24.0
    DateTime.parse("01:02:03 I").offset.should == 9/24.0
    DateTime.parse("01:02:03 K").offset.should == 10/24.0
    DateTime.parse("01:02:03 L").offset.should == 11/24.0
    DateTime.parse("01:02:03 M").offset.should == 12/24.0
    DateTime.parse("01:02:03 N").offset.should == -1/24.0
    DateTime.parse("01:02:03 O").offset.should == -2/24.0
    DateTime.parse("01:02:03 P").offset.should == -3/24.0
    DateTime.parse("01:02:03 Q").offset.should == -4/24.0
    DateTime.parse("01:02:03 R").offset.should == -5/24.0
    DateTime.parse("01:02:03 S").offset.should == -6/24.0
    DateTime.parse("01:02:03 T").offset.should == -7/24.0
    DateTime.parse("01:02:03 U").offset.should == -8/24.0
    DateTime.parse("01:02:03 V").offset.should == -9/24.0
    DateTime.parse("01:02:03 W").offset.should == -10/24.0
    DateTime.parse("01:02:03 X").offset.should == -11/24.0
    DateTime.parse("01:02:03 Y").offset.should == -12/24.0
    DateTime.parse("01:02:03 Z").offset.should == 0
  end

  it "should parse the time strings output by ruby's Time class" do
    proc{DateTime.parse(Time.now.to_s)}.should_not raise_error
  end

  it "can handle a leftover DD as an hour if the day has already been parsed" do
    DateTime.parse("2009-12-13 10").should == DateTime.civil(2009, 12, 13, 10)
  end

  it "can handle DD as month day number" do
    DateTime.parse("10").should == DateTime.civil(Date.today.year, Date.today.month, 10)
    DateTime.parse("10 01:02:03").should == DateTime.civil(Date.today.year, Date.today.month, 10, 1, 2, 3)
  end

  it "can handle DDz as day and zone" do
    DateTime.parse("10z").should == DateTime.civil(Date.today.year, Date.today.month, 10)
  end

  it "can handle DD+DD as day and zone" do
    DateTime.parse("10+11").should == DateTime.civil(Date.today.year, Date.today.month, 10, 0, 0, 0, 11/24.0)
  end

  it "can handle DD[DDDD] as day and zone" do
    DateTime.parse("10[1112]").should == DateTime.civil(Date.today.year, Date.today.month, 10, 0, 0, 0, 11.2/24.0)
  end

  it "can handle DD.D as second" do
    DateTime.parse("11.1").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 0, 0, 11) + 0.1/86400
  end

  it "can handle DDtDD as hour" do
    DateTime.parse("11t12").should == DateTime.civil(Date.today.year, Date.today.month, 11, 12, 0, 0)
  end

  it "can handle DDtDDDD as hour and minute" do
    DateTime.parse("11t1213").should == DateTime.civil(Date.today.year, Date.today.month, 11, 12, 13, 0)
  end

  it "can handle DDtDDDDDD as hour, minute, and second" do
    DateTime.parse("11t121314").should == DateTime.civil(Date.today.year, Date.today.month, 11, 12, 13, 14)
  end

  it "can handle DDtDDDDDD.D as hour, minute, and second" do
    DateTime.parse("11t121314.1").should == DateTime.civil(Date.today.year, Date.today.month, 11, 12, 13, 14) + 0.1/86400
  end

  it "can handle DDD as year day number" do
    DateTime.parse("050").should == DateTime.civil(Date.today.year, 2, 19)
    DateTime.parse("050 1:02:03").should == DateTime.civil(Date.today.year, 2, 19, 1, 2, 3)
  end

  it "can handle DDD.D as minute and second" do
    DateTime.parse("211.1").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 0, 2, 11) + 0.1/86400
  end

  it "can handle MMDD as month and day" do
    DateTime.parse("1108").should == DateTime.civil(Date.today.year, 11, 8)
    DateTime.parse("1108 10:02:03").should == DateTime.civil(Date.today.year, 11, 8, 10, 2, 3)
  end

  it "can handle DDDD.D as minute and second" do
    DateTime.parse("1211.1").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 0, 12, 11) + 0.1/86400
  end

  it "can handle YYDDD as year and day number" do
    DateTime.parse("10100").should == DateTime.civil(2010, 4, 10)
    DateTime.parse("10100 23:02:03").should == DateTime.civil(2010, 4, 10, 23, 2, 3)
  end

  it "can handle DDDDD.D as hour, minute, and second" do
    DateTime.parse("31211.1").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 3, 12, 11) + 0.1/86400
  end

  it "can handle YYMMDD as year month and day" do
    DateTime.parse("201023").should == DateTime.civil(2020, 10, 23)
    DateTime.parse("201023 23:02:03 +0800").should == DateTime.civil(2020, 10, 23, 23, 2, 3, 8/24.0)
  end

  it "can handle DDDDDD.D as hour, minute, and second" do
    DateTime.parse("131211.1").should == DateTime.civil(Date.today.year, Date.today.month, Date.today.day, 13, 12, 11) + 0.1/86400
  end

  it "can handle YYYYDDD as year and day number" do
    DateTime.parse("1910100").should == DateTime.civil(1910, 4, 10)
    DateTime.parse("1910100 23:02:03 -0101").should == DateTime.civil(1910, 4, 10, 23, 2, 3, -61/1440.0)
  end

  it "can handle DDDDDDD.D as day, hour, minute, and second" do
    DateTime.parse("4131211.1").should == DateTime.civil(Date.today.year, Date.today.month, 4, 13, 12, 11) + 0.1/86400
  end

  it "can handle YYYYMMDD as year and day number" do
    DateTime.parse("19101101").should == DateTime.civil(1910, 11, 1)
    DateTime.parse("19101101T23:02:03 +0000").should == DateTime.civil(1910, 11, 1, 23, 2, 3)
  end

  it "can handle DDDDDDDD.D as day, hour, minute, and second" do
    DateTime.parse("14131211.1").should == DateTime.civil(Date.today.year, Date.today.month, 14, 13, 12, 11) + 0.1/86400
  end

  it "can handle DDDDDDDD.D as month, day, hour, minute, and second" do
    DateTime.parse("1014131211.1").should == DateTime.civil(Date.today.year, 10, 14, 13, 12, 11) + 0.1/86400
  end

  it "can handle DDDDDDDDDD.D as year, month, day, hour, minute, and second" do
    DateTime.parse("091014131211.1").should == DateTime.civil(2009, 10, 14, 13, 12, 11) + 0.1/86400
  end

  it "can handle DDDDDDDDDDDD.D as year, month, day, hour, minute, and second" do
    DateTime.parse("20091014131211.1").should == DateTime.civil(2009, 10, 14, 13, 12, 11) + 0.1/86400
  end

end

describe :date_parse, :shared => true do
  it "can parse a mmm-YYYY string into a DateTime object" do
    d = DateTime.parse("feb#{@sep}2008")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 1
    
    d = DateTime.parse("feb#{@sep}2008  1:02:03")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 1
    d.hour.should  == 1
    d.min.should   == 2
    d.sec.should   == 3
  end

  it "can parse a 'DD mmm YYYY' string into a DateTime object" do
    d = DateTime.parse("23#{@sep}feb#{@sep}2008")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 23
    
    d = DateTime.parse("23#{@sep}feb#{@sep}2008 11:02:03")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 23
    d.hour.should  == 11
    d.min.should   == 2
    d.sec.should   == 3
  end

  it "can parse a 'mmm DD YYYY' string into a DateTime object" do
    d = DateTime.parse("feb#{@sep}23#{@sep}2008")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 23
    
    d = DateTime.parse("feb#{@sep}23#{@sep}2008 01:02:03")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 23
    d.hour.should  == 1
    d.min.should   == 2
    d.sec.should   == 3
  end

  it "can parse a 'YYYY mmm DD' string into a DateTime object" do
    d = DateTime.parse("2008#{@sep}feb#{@sep}23")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 23
    
    d = DateTime.parse("2008#{@sep}feb#{@sep}23 01:02")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 23
    d.hour.should  == 1
    d.min.should   == 2
    d.sec.should   == 0
  end

  it "can parse a month name and day into a Date object" do
    DateTime.parse("november#{@sep}5th").should == DateTime.civil(Date.today.year, 11, 5)
    DateTime.parse("november#{@sep}5th 1:02").should == DateTime.civil(Date.today.year, 11, 5, 1, 2)
  end

  it "can parse a month name, day and year into a Date object" do
    DateTime.parse("november#{@sep}5th#{@sep}2005").should == DateTime.civil(2005, 11, 5)
    DateTime.parse("november#{@sep}5th#{@sep}2005  1:02").should == DateTime.civil(2005, 11, 5, 1, 2)
  end

  it "can parse a year, month name and day into a Date object" do
    DateTime.parse("2005#{@sep}november#{@sep}5th").should == DateTime.civil(2005, 11, 5)
    DateTime.parse("2005#{@sep}november#{@sep}5th 01:02 +0100").should == DateTime.civil(2005, 11, 5, 1, 2, 0, 1/24.0)
  end

  it "can parse a year, day and month name into a Date object" do
    DateTime.parse("5th#{@sep}november#{@sep}2005").should == DateTime.civil(2005, 11, 5)
    DateTime.parse("5th#{@sep}november#{@sep}2005  1:02 -0100").should == DateTime.civil(2005, 11, 5, 1, 2, 0, -1/24.0)
  end

  it "can handle negative year numbers" do
    DateTime.parse("5th#{@sep}november#{@sep}-2005").should == DateTime.civil(-2005, 11, 5)
    DateTime.parse("5th#{@sep}november#{@sep}-2005 1:02 -0100").should == DateTime.civil(-2005, 11, 5, 1, 2, 0, -1/24.0)
  end
end

describe :date_parse_us, :shared => true do
  it "parses a YYYY#{@sep}MM#{@sep}DD string into a DateTime object" do
    d = DateTime.parse("2007#{@sep}10#{@sep}01")
    d.year.should  == 2007
    d.month.should == 10
    d.day.should   == 1
    
    d = DateTime.parse("2007#{@sep}10#{@sep}01 01:02:03")
    d.year.should  == 2007
    d.month.should == 10
    d.day.should   == 1
    d.hour.should  == 1
    d.min.should   == 2
    d.sec.should   == 3
  end

  ruby_version_is "" ... "1.9" do
    it "parses a MM#{@sep}DD#{@sep}YYYY string into a DateTime object" do
      d = DateTime.parse("10#{@sep}01#{@sep}2007")
      d.year.should  == 2007
      d.month.should == 10
      d.day.should   == 1
      
      
      d = DateTime.parse("10#{@sep}01#{@sep}2007 01:02:03")
      d.year.should  == 2007
      d.month.should == 10
      d.day.should   == 1
      d.hour.should  == 1
      d.min.should   == 2
      d.sec.should   == 3
    end

    it "parses a MM#{@sep}DD#{@sep}YY string into a DateTime object using the year digits as 20XX" do
      d = DateTime.parse("10#{@sep}01#{@sep}07")
      d.year.should  == 2007
      d.month.should == 10
      d.day.should   == 1
      
      d = DateTime.parse("10#{@sep}01#{@sep}97 01:02:03 Z")
      d.year.should  == 1997
      d.month.should == 10
      d.day.should   == 1
      d.hour.should  == 1
      d.min.should   == 2
      d.sec.should   == 3
    end
  end

  ruby_version_is "1.9" do
    it "parses a DD#{@sep}MM#{@sep}YYYY string into a DateTime object" do
      d = DateTime.parse("10#{@sep}01#{@sep}2007")
      d.year.should  == 2007
      d.month.should == 1
      d.day.should   == 10
      
      
      d = DateTime.parse("10#{@sep}01#{@sep}2007 01:02:03")
      d.year.should  == 2007
      d.month.should == 1
      d.day.should   == 10
      d.hour.should  == 1
      d.min.should   == 2
      d.sec.should   == 3
    end

    it "parses a YY#{@sep}MM#{@sep}DD string into a DateTime object using the year digits as 20XX" do
      d = DateTime.parse("10#{@sep}01#{@sep}07")
      d.year.should  == 2010
      d.month.should == 1
      d.day.should   == 7
      
      d = DateTime.parse("10#{@sep}01#{@sep}07 01:02:03 Z")
      d.year.should  == 2010
      d.month.should == 1
      d.day.should   == 7
      d.hour.should  == 1
      d.min.should   == 2
      d.sec.should   == 3
    end
  end
end

describe :date_parse_eu, :shared => true do
  # The - separator let's it work like European format, so it as a different spec
  it "can parse a YYYY-MM-DD string into a DateTime object" do
    d = DateTime.parse("2007#{@sep}10#{@sep}01")
    d.year.should  == 2007
    d.month.should == 10
    d.day.should   == 1
    
    d = DateTime.parse("2007#{@sep}10#{@sep}01 01:02:03Z")
    d.year.should  == 2007
    d.month.should == 10
    d.day.should   == 1
    d.hour.should  == 1
    d.min.should   == 2
    d.sec.should   == 3
  end

  it "can parse a DD-MM-YYYY string into a DateTime object" do
    d = DateTime.parse("10#{@sep}01#{@sep}2007")
    d.year.should  == 2007
    d.month.should == 1
    d.day.should   == 10
    
    d = DateTime.parse("10#{@sep}01#{@sep}2007 01:02:03-01:00")
    d.year.should  == 2007
    d.month.should == 1
    d.day.should   == 10
    d.hour.should  == 1
    d.min.should   == 2
    d.sec.should   == 3
    d.offset.should == -1/24.0
  end

  it "can parse a YY-MM-DD string into a DateTime object" do
    d = DateTime.parse("10#{@sep}01#{@sep}07")
    d.year.should  == 2010
    d.month.should == 1
    d.day.should   == 7
    
    d = DateTime.parse("97#{@sep}01#{@sep}07 01:02:03+01:00")
    d.year.should  == 1997
    d.month.should == 1
    d.day.should   == 7
    d.hour.should  == 1
    d.min.should   == 2
    d.sec.should   == 3
    d.offset.should == 1/24.0
  end
end


describe "DateTime#parse with '.' separator" do
  before :all do
    @sep = '.'
  end

  it_should_behave_like "date_parse"
end

describe "DateTime#parse with '/' separator" do
  before :all do
    @sep = '/'
  end

  it_should_behave_like "date_parse"
end

describe "DateTime#parse with ' ' separator" do
  before :all do
    @sep = ' '
  end

  it_should_behave_like "date_parse"
end

describe "DateTime#parse with '/' separator US-style" do
  before :all do
    @sep = '/'
  end

  it_should_behave_like "date_parse_us"
end

ruby_version_is "" ... "1.8.7" do
  describe "DateTime#parse with '.' separator US-style" do
    before :all do
      @sep = '.'
    end

    it_should_behave_like "date_parse_us"
  end
end

describe "DateTime#parse with '-' separator EU-style" do
  before :all do
    @sep = '-'
  end

  it_should_behave_like "date_parse_eu"
end
