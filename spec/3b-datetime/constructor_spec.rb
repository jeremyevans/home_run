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
  
  it ".jd creates a datetime with arguments" do
    d = DateTime.jd(2000, 6, 7, 8, 1/24.0)
    d.jd.should == 2000
    d.hour.should == 6
    d.min.should == 7
    d.sec.should == 8
    d.offset.should == 1/24.0
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
end
