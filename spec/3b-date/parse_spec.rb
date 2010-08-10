require File.expand_path('../../spec_helper', __FILE__)

describe "Date#parse" do
  it "can't handle a empty string" do
    lambda{ Date.parse("") }.should raise_error(ArgumentError)
  end

  # Specs using numbers
  it "can't handle a single digit" do
    lambda{ Date.parse("1") }.should raise_error(ArgumentError)
  end

  it "can handle DD as month day number" do
    Date.parse("10").should == Date.civil(Date.today.year, Date.today.month, 10)
  end

  it "can handle DDD as year day number" do
    Date.parse("050").should == Date.civil(Date.today.year, 2, 19)
  end

  it "can handle MMDD as month and day" do
    Date.parse("1108").should == Date.civil(Date.today.year, 11, 8)
  end

  it "can handle YYDDD as year and day number" do
    Date.parse("10100").should == Date.civil(2010, 4, 10)
  end

  it "can handle YYMMDD as year month and day" do
    Date.parse("201023").should == Date.civil(2020, 10, 23)
  end

  it "can handle YYYYDDD as year and day number" do
    Date.parse("1910100").should == Date.civil(1910, 4, 10)
  end

  it "can handle YYYYMMDD as year and day number" do
    Date.parse("19101101").should == Date.civil(1910, 11, 1)
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
end

describe :date_parse_eu, :shared => true do
  # The - separator let's it work like European format, so it as a different spec
  it "can parse a YYYY-MM-DD string into a Date object" do
    d = Date.parse("2007#{@sep}10#{@sep}01")
    d.year.should  == 2007
    d.month.should == 10
    d.day.should   == 1
  end

  it "can parse a DD-MM-YYYY string into a Date object" do
    d = Date.parse("10#{@sep}01#{@sep}2007")
    d.year.should  == 2007
    d.month.should == 1
    d.day.should   == 10
  end

  it "can parse a YY-MM-DD string into a Date object" do
    d = Date.parse("10#{@sep}01#{@sep}07")
    d.year.should  == 2010
    d.month.should == 1
    d.day.should   == 7
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
  before :all do
    @sep = '-'
  end

  it_should_behave_like "date_parse_eu"
end
