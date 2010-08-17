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
  
  it ".jd should have defaults and an optional sg value" do
    DateTime.jd.should == DateTime.jd(0, 0, 0, 0, 0)
    DateTime.jd(0).should == DateTime.jd(0, 0, 0, 0, 0)
    DateTime.jd(0, 0).should == DateTime.jd(0, 0, 0, 0, 0)
    DateTime.jd(0, 0, 0).should == DateTime.jd(0, 0, 0, 0, 0)
    DateTime.jd(0, 0, 0, 0).should == DateTime.jd(0, 0, 0, 0, 0)
    DateTime.jd(0, 0, 0, 0, 0).should == DateTime.jd(0, 0, 0, 0, 0)
    DateTime.jd(2008, 1, 1, 1, 0.5, 1).should == DateTime.jd(2008, 1, 1, 1, 0.5)
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
    DateTime.ordinal(2008, 1, 1, 1, 1, 0.5, 1).should == DateTime.ordinal(2008, 1, 1, 1, 1, 0.5)
    DateTime.ordinal.should == DateTime.jd
  end
end
