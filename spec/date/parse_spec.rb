require File.expand_path('../../spec_helper', __FILE__)

describe "Date#parse" do
  ruby_version_is "" ... "1.9" do
    it "#zone_to_diff? should return the offset in seconds if a valid time zone, or 0 if not" do
      Date.zone_to_diff('-0800').should == -28800
      Date.zone_to_diff('+08:00').should == 28800
      Date.zone_to_diff('YY').should == 0
    end
  end
  
  it "._parse should parse the date string and return a hash" do
    Date._parse('2008-10-20 11:12:13.0-08:00').should == {:mon=>10, :zone=>'-08:00', :sec=>13, :sec_fraction=>0.0, :year=>2008, :hour=>11, :offset=>-28800, :mday=>20, :min=>12}
    Date._parse('2008-10-20 11:12:13.0-08:00', true).should == {:mon=>10, :zone=>'-08:00', :sec=>13, :sec_fraction=>0.0, :year=>2008, :hour=>11, :offset=>-28800, :mday=>20, :min=>12}
  end

  it "._parse should not contain fields in the hash that were guessed" do
    Date._parse('2008-10-20').should == {:year=>2008, :mday=>20, :mon=>10}
    Date._parse('11:12:13').should == {:sec=>13, :hour=>11, :min=>12}
  end
  
  it "should have defaults and an optional sg value" do
    Date.parse.should == Date.jd
    Date.parse('2008-10-11').should == Date.civil(2008, 10, 11)
    Date.parse('2008-10-11', true).should == Date.civil(2008, 10, 11)
    Date.parse('2008-10-11', true, 1).should == Date.civil(2008, 10, 11)
  end
  
  it "raises errors for invalid dates" do
    lambda { Date.parse("") }.should raise_error(ArgumentError)
    lambda { Date.parse("2009-02-29") }.should raise_error(ArgumentError)
  end

  # The space separator is also different, doesn't work for only numbers
  it "can parse a day name into a Date object" do
    d = Date.parse("friday")
    d.should == Date.commercial(d.cwyear, d.cweek, 5)
  end

  it "can parse a month name into a Date object" do
    d = Date.parse("october")
    d.should == Date.civil(Date.today.year, 10)
  end

  it "can parse a month day into a Date object" do
    d = Date.parse("5th")
    d.should == Date.civil(Date.today.year, Date.today.month, 5)
  end
  
  # Specs using numbers
  it "can't handle a single digit" do
    lambda{ Date.parse("1") }.should raise_error(ArgumentError)
  end

  it "can handle DD as month day number" do
    d = Date.parse("10")
    d.should == Date.civil(Date.today.year, Date.today.month, 10)
  end

  it "can handle DDD as year day number" do
    d = Date.parse("100")
    if Date.gregorian_leap?(Date.today.year)
      d.should == Date.civil(Date.today.year, 4, 9)
    else
      d.should == Date.civil(Date.today.year, 4, 10)
    end
  end

  it "can handle MMDD as month and day" do
    d = Date.parse("1108")
    d.should == Date.civil(Date.today.year, 11, 8)
  end

  it "can handle YYDDD as year and day number in 1969--2068" do
    d = Date.parse("10100")
    d.should == Date.civil(2010, 4, 10)
  end

  it "can handle YYMMDD as year month and day in 1969--2068" do
    d = Date.parse("201023")
    d.should == Date.civil(2020, 10, 23)
  end
  
  it "can handle YYDDD as a 2 digit year and day number" do
    d = Date.parse("10100", false)
    d.should == Date.civil(10, 4, 10)
  end

  it "can handle YYMMDD as 2 digit year month and day" do
    d = Date.parse("201023", false)
    d.should == Date.civil(20, 10, 23)
  end

  it "can handle YYYYDDD as year and day number" do
    d = Date.parse("1910100")
    d.should == Date.civil(1910, 4, 10)
  end

  it "can handle YYYYMMDD as year and day number" do
    d = Date.parse("19101101")
    d.should == Date.civil(1910, 11, 1)
  end
end

describe :date_parse, :shared => true do
  it "can parse a mmm-YYYY string into a Date object" do
    d = Date.parse("feb#{@sep}2008")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 1
  end

  it "can parse a 'DD mmm YYYY' string into a Date object" do
    d = Date.parse("23#{@sep}feb#{@sep}2008")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 23
  end

  it "can parse a 'mmm DD YYYY' string into a Date object" do
    d = Date.parse("feb#{@sep}23#{@sep}2008")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 23

    d = Date.parse("feb#{@sep}23,#{@sep}2008")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 23

    d = Date.parse("feb#{@sep}23,#{@sep}08")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 23
  end

  it "can parse a 'YYYY mmm DD' string into a Date object" do
    d = Date.parse("2008#{@sep}feb#{@sep}23")
    d.year.should  == 2008
    d.month.should == 2
    d.day.should   == 23
  end

  it "can parse a month name and day into a Date object" do
    Date.parse("november#{@sep}5th").should == Date.civil(Date.today.year, 11, 5)
  end

  it "can parse a weekday, month, day, and year into a Date object" do
    Date.parse("Mon Aug 10 2009").should == Date.civil(2009, 8, 10)
  end

  it "can parse a month name, day and year into a Date object" do
    Date.parse("november#{@sep}5th#{@sep}2005").should == Date.civil(2005, 11, 5)
  end

  it "can parse a year, month name and day into a Date object" do
    Date.parse("2005#{@sep}november#{@sep}5th").should == Date.civil(2005, 11, 5)
  end

  it "can parse a year, day and month name into a Date object" do
    Date.parse("5th#{@sep}november#{@sep}2005").should == Date.civil(2005, 11, 5)
  end

  it "can handle negative year numbers" do
    Date.parse("5th#{@sep}november#{@sep}-2005").should == Date.civil(-2005, 11, 5)
  end
end

describe :date_parse_us, :shared => true do
  it "parses a YYYY#{@sep}MM#{@sep}DD string into a Date object" do
    d = Date.parse("2007#{@sep}10#{@sep}01")
    d.year.should  == 2007
    d.month.should == 10
    d.day.should   == 1
  end

  ruby_version_is "" ... "1.9" do
    it "parses a MM#{@sep}DD#{@sep}YYYY string into a Date object" do
      d = Date.parse("10#{@sep}01#{@sep}2007")
      d.year.should  == 2007
      d.month.should == 10
      d.day.should   == 1
    end

    it "parses a MM#{@sep}DD#{@sep}YY string into a Date object using the year digits as 20XX" do
      d = Date.parse("10#{@sep}01#{@sep}07")
      d.year.should  == 2007
      d.month.should == 10
      d.day.should   == 1
    end
    
    it "parses a MM#{@sep}DD#{@sep}YY string into a Date object using the year digits as 20XX" do
      d = Date.parse("10#{@sep}01#{@sep}07", true)
      d.year.should  == 2007
      d.month.should == 10
      d.day.should   == 1
    end
    
    it "parses a MM#{@sep}DD#{@sep}YY string into a Date object NOT using the year digits as 20XX" do
      d = Date.parse("10#{@sep}01#{@sep}07", false)
      d.year.should  == 7
      d.month.should == 10
      d.day.should   == 1
    end
  end

  ruby_version_is "1.9" do
    it "parses a DD#{@sep}MM#{@sep}YYYY string into a Date object" do
      d = Date.parse("10#{@sep}01#{@sep}2007")
      d.year.should  == 2007
      d.month.should == 1
      d.day.should   == 10
    end

    it "parses a YY#{@sep}MM#{@sep}DD string into a Date object using the year digits as 20XX" do
      d = Date.parse("10#{@sep}01#{@sep}07")
      d.year.should  == 2010
      d.month.should == 1
      d.day.should   == 7
    end
    
    it "parses a YY#{@sep}MM#{@sep}DD string into a Date object NOT using the year digits as 20XX" do
      d = Date.parse("10#{@sep}01#{@sep}07", false)
      d.year.should  == 10
      d.month.should == 1
      d.day.should   == 7
    end

    it "parses a YY#{@sep}MM#{@sep}DD string into a Date object using the year digits as 20XX" do
      d = Date.parse("10#{@sep}01#{@sep}07", true)
      d.year.should  == 2010
      d.month.should == 1
      d.day.should   == 7
    end
  end
end

describe "Date#parse with '.' separator" do
  before :all do
    @sep = '.'
  end

  it_should_behave_like "date_parse"
end

describe "Date#parse with '/' separator" do
  before :all do
    @sep = '/'
  end

  it_should_behave_like "date_parse"
end

describe "Date#parse with ' ' separator" do
  before :all do
    @sep = ' '
  end

  it_should_behave_like "date_parse"
end

describe "Date#parse with '/' separator US-style" do
  before :all do
    @sep = '/'
  end

  it_should_behave_like "date_parse_us"
end

ruby_version_is "" ... "1.8.7" do
  describe "Date#parse with '.' separator US-style" do
    before :all do
      @sep = '.'
    end

    it_should_behave_like "date_parse_us"
  end
end

describe "Date#parse with '-' separator EU-style" do
  # The - separator let's it work like European format, so it as a different spec
  it "can parse a YYYY-MM-DD string into a Date object" do
    d = Date.parse("2007-10-01")
    d.year.should  == 2007
    d.month.should == 10
    d.day.should   == 1
  end

  it "can parse a DD-MM-YYYY string into a Date object" do
    d = Date.parse("10-01-2007")
    d.year.should  == 2007
    d.month.should == 1
    d.day.should   == 10
  end

  it "can parse a YY-MM-DD string into a Date object" do
    d = Date.parse("10-01-07")
    d.year.should  == 2010
    d.month.should == 1
    d.day.should   == 7
  end

  it "can parse a YY-DD-MM string into a Date object" do
    d = Date.parse("10-01-07")
    d.year.should  == 2010
    d.month.should == 1
    d.day.should   == 7
  end

  it "can parse a MM-DD-YY string into a Date object NOT using the year digits as 20XX" do
    d = Date.parse("10-01-07", false)
    d.year.should  == 10
    d.month.should == 1
    d.day.should   == 7
  end

  it "can parse a YY-MM-DD string into a Date object using the year digits as 20XX" do
    d = Date.parse("10-01-07", true)
    d.year.should  == 2010
    d.month.should == 1
    d.day.should   == 7
  end
end

ruby_version_is "1.8.7" do
  describe "Date#parse(.)" do
    it "parses a YYYY.MM.DD string into a Date object" do
      d = Date.parse("2007.10.01")
      d.year.should  == 2007
      d.month.should == 10
      d.day.should   == 1
    end

    it "parses a DD.MM.YYYY string into a Date object" do
      d = Date.parse("10.01.2007")
      d.year.should  == 2007
      d.month.should == 1
      d.day.should   == 10
    end

    ruby_version_is "" ... "1.9" do
      it "parses a YY.MM.DD string into a Date object" do
        d = Date.parse("10.01.07")
        d.year.should  == 2010
        d.month.should == 1
        d.day.should   == 7
      end
    end

    ruby_version_is "1.9" do
      it "parses a YY.MM.DD string into a Date object" do
        d = Date.parse("10.01.07")
        d.year.should  == 2010
        d.month.should == 1
        d.day.should   == 7
      end
    end

    it "parses a YY.MM.DD string into a Date object using the year digits as 20XX" do
      d = Date.parse("10.01.07", true)
      d.year.should  == 2010
      d.month.should == 1
      d.day.should   == 7
    end
  end
end
