require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime constructors" do
  it ".civil creates a datetime with arguments" do
    d = DateTime.civil(2000, 3, 5, 6, 7, 8, 1/24.0)
    d.year.should == 2000
    d.month.should == 3
    d.day.should == 5
    d.hour.should == 6
    d.min.should == 7
    d.sec.should == 8
    d.offset.should == 1/24.0
  end
  
  it ".civil should have defaults and an optional sg value" do
    DateTime.civil.should == DateTime.jd
    DateTime.civil(2008).should == DateTime.civil(2008, 1, 1, 0, 0, 0, 0.0)
    DateTime.civil(2008, 1).should == DateTime.civil(2008, 1, 1, 0, 0, 0, 0.0)
    DateTime.civil(2008, 1, 1).should == DateTime.civil(2008, 1, 1, 0, 0, 0, 0.0)
    DateTime.civil(2008, 1, 1, 0).should == DateTime.civil(2008, 1, 1, 0, 0, 0, 0.0)
    DateTime.civil(2008, 1, 1, 0, 0).should == DateTime.civil(2008, 1, 1, 0, 0, 0, 0.0)
    DateTime.civil(2008, 1, 1, 0, 0, 0).should == DateTime.civil(2008, 1, 1, 0, 0, 0, 0.0)
    DateTime.civil(2008, 1, 1, 0, 0, 0, 0.0).should == DateTime.civil(2008, 1, 1, 0, 0, 0, 0.0)
    DateTime.civil(2008, 1, 1, 1, 1, 1, 0.5, 1).should == DateTime.civil(2008, 1, 1, 1, 1, 1, 0.5)
  end

  it ".commercial creates a datetime with arguments" do
    d = DateTime.commercial(2000, 3, 5, 6, 7, 8, 1/24.0)
    d.cwyear.should == 2000
    d.cweek.should == 3
    d.cwday.should == 5
    d.hour.should == 6
    d.min.should == 7
    d.sec.should == 8
    d.offset.should == 1/24.0
  end
  
  ruby_version_is "1.9" do
    it ".commercial should have defaults and an optional sg value" do
      DateTime.commercial.should == DateTime.jd
      DateTime.commercial(2008).should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1).should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1, 1).should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1, 1, 0).should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1, 1, 0, 0).should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1, 1, 0, 0, 0).should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0).should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1, 1, 1, 1, 1, 0.5, 1).should == DateTime.commercial(2008, 1, 1, 1, 1, 1, 0.5)
    end
  end

  ruby_version_is "" ... "1.9" do
    it ".commercial should have defaults and an optional sg value" do
      DateTime.commercial.should == DateTime.commercial(1582, 41, 5)
      DateTime.commercial(2008).should == DateTime.commercial(2008, 41, 5, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1).should == DateTime.commercial(2008, 1, 5, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1, 1).should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1, 1, 0).should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1, 1, 0, 0).should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1, 1, 0, 0, 0).should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0).should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.0)
      DateTime.commercial(2008, 1, 1, 1, 1, 1, 0.5, 1).should == DateTime.commercial(2008, 1, 1, 1, 1, 1, 0.5)
    end
  end

  it ".jd creates a datetime with arguments" do
    d = DateTime.jd(2000, 6, 7, 8, 1/24.0)
    d.jd.should == 2000
    d.hour.should == 6
    d.min.should == 7
    d.sec.should == 8
    d.offset.should == 1/24.0
  end
  
  it ".jd handles a fractional date as first argument" do
    d = DateTime.jd(2000.5)
    d.jd.should == 2000
    d.hour.should == 12
    d.min.should == 0
    d.sec.should == 0
    d.offset.should == 0
  end
  
  it ".jd should raise an error if given a float as first argument and any additional arguments" do
    proc{DateTime.jd(2000.5, 1)}.should raise_error(ArgumentError)
  end

  it ".jd should have defaults and an optional sg value" do
    DateTime.jd.should == DateTime.jd(0, 0, 0, 0, 0)
    DateTime.jd(0).should == DateTime.jd(0, 0, 0, 0, 0)
    DateTime.jd(0, 0).should == DateTime.jd(0, 0, 0, 0, 0)
    DateTime.jd(0, 0, 0).should == DateTime.jd(0, 0, 0, 0, 0)
    DateTime.jd(0, 0, 0, 0).should == DateTime.jd(0, 0, 0, 0, 0)
    DateTime.jd(0, 0, 0, 0, 0).should == DateTime.jd(0, 0, 0, 0, 0)
    DateTime.jd(2008, 1, 1, 1, 0.5, 1).should == DateTime.jd(2008, 1, 1, 1, 0.5)
  end
  
  it ".new! creates a datetime with arguments" do
    d = DateTime.new!(2422222)
    d.jd.should == 2422222
    d.hour.should == 12
    d.min.should == 0
    d.sec.should == 0
    d.offset.should == 0

    d = DateTime.new!(2422222.5)
    d.jd.should == 2422223
    d.hour.should == 0
    d.min.should == 0
    d.sec.should == 0
    d.offset.should == 0

    d = DateTime.new!(2422222, 0.5)
    d.jd.should == 2422223
    d.hour.should == 0
    d.min.should == 0
    d.sec.should == 0
    d.offset.should == 0.5

    d = DateTime.new!(2422222.5, 0.5)
    d.jd.should == 2422223
    d.hour.should == 12
    d.min.should == 0
    d.sec.should == 0
    d.offset.should == 0.5
  end
  
  it ".new! should have defaults and an optional sg value" do
    DateTime.new!.should == DateTime.jd
    DateTime.new!(0).should == DateTime.new!(0, 0)
    DateTime.new!(0, 0).should == DateTime.new!(0, 0)
    DateTime.new!(2008, 0.5, 1).should == DateTime.new!(2008, 0.5)
  end
  
  it ".ordinal creates a datetime with arguments" do
    d = DateTime.ordinal(2000, 100, 6, 7, 8, 1/24.0)
    d.year.should == 2000
    d.yday.should == 100
    d.hour.should == 6
    d.min.should == 7
    d.sec.should == 8
    d.offset.should == 1/24.0
  end
  
  it ".ordinal should have defaults and an optional sg value" do
    DateTime.ordinal.should == DateTime.jd
    DateTime.ordinal(2008).should == DateTime.ordinal(2008, 1, 0, 0, 0, 0)
    DateTime.ordinal(2008, 1).should == DateTime.ordinal(2008, 1, 0, 0, 0, 0)
    DateTime.ordinal(2008, 1, 0).should == DateTime.ordinal(2008, 1, 0, 0, 0, 0)
    DateTime.ordinal(2008, 1, 0, 0).should == DateTime.ordinal(2008, 1, 0, 0, 0, 0)
    DateTime.ordinal(2008, 1, 0, 0, 0).should == DateTime.ordinal(2008, 1, 0, 0, 0, 0)
    DateTime.ordinal(2008, 1, 0, 0, 0, 0).should == DateTime.ordinal(2008, 1, 0, 0, 0, 0)
    DateTime.ordinal(2008, 1, 1, 1, 1, 0.5, 1).should == DateTime.ordinal(2008, 1, 1, 1, 1, 0.5)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(DateTime)
    c.jd.should be_kind_of(c)
    c.civil.should be_kind_of(c)
    c.commercial.should be_kind_of(c)
    c.ordinal.should be_kind_of(c)
    c.new!.should be_kind_of(c)
    c.new.should be_kind_of(c)
  end

  it "should handle strings for the offset" do
    DateTime.jd(2008, 0, 0, 0, 'Z').should == DateTime.jd(2008, 0, 0, 0, 0)
    DateTime.civil(2008, 1, 1, 0, 0, 0, 'Z').should == DateTime.civil(2008, 1, 1, 0, 0, 0, 0)
    DateTime.commercial(2008, 1, 1, 0, 0, 0, 'Z').should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0)
    DateTime.ordinal(2008, 1, 0, 0, 0, 'Z').should == DateTime.ordinal(2008, 1, 0, 0, 0, 0)
    DateTime.new(2008, 1, 1, 0, 0, 0, 'Z').should == DateTime.new(2008, 1, 1, 0, 0, 0, 0)

    DateTime.jd(2008, 0, 0, 0, '+1200').should == DateTime.jd(2008, 0, 0, 0, 0.5)
    DateTime.civil(2008, 1, 1, 0, 0, 0, '+1200').should == DateTime.civil(2008, 1, 1, 0, 0, 0, 0.5)
    DateTime.commercial(2008, 1, 1, 0, 0, 0, '+1200').should == DateTime.commercial(2008, 1, 1, 0, 0, 0, 0.5)
    DateTime.ordinal(2008, 1, 0, 0, 0, '+1200').should == DateTime.ordinal(2008, 1, 0, 0, 0, 0.5)
    DateTime.new(2008, 1, 1, 0, 0, 0, '+1200').should == DateTime.new(2008, 1, 1, 0, 0, 0, 0.5)
  end

  it "should handle fractional seconds in seconds" do
    DateTime.jd(2008, 0, 0, 1.5).should == DateTime.jd(2008, 0, 0, 1) + (0.5/86400)
    DateTime.civil(2008, 1, 1, 0, 0, 1.5).should == DateTime.civil(2008, 1, 1, 0, 0, 1) + (0.5/86400)
    DateTime.commercial(2008, 1, 1, 0, 0, 1.5).should == DateTime.commercial(2008, 1, 1, 0, 0, 1) + (0.5/86400)
    DateTime.ordinal(2008, 1, 0, 0, 1.5).should == DateTime.ordinal(2008, 1, 0, 0, 1) + (0.5/86400)
    DateTime.new(2008, 1, 1, 0, 0, 1.5).should == DateTime.new(2008, 1, 1, 0, 0, 1) + (0.5/86400)
  end
end
