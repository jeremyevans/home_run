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
    DateTime.civil(2008, 1, 1, 1, 1, 1, 0.5, 1).should == DateTime.civil(2008, 1, 1, 1, 1, 1, 0.5)
    DateTime.civil.should == DateTime.jd
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
  
  it ".commercial should have defaults and an optional sg value" do
    DateTime.commercial(2008, 1, 1, 1, 1, 1, 0.5, 1).should == DateTime.commercial(2008, 1, 1, 1, 1, 1, 0.5)
    DateTime.commercial.should == (RUBY_VERSION > '1.9.0' ? DateTime.jd : DateTime.commercial(1582, 41, 5))
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
    DateTime.jd(2008, 1, 1, 1, 0.5, 1).should == DateTime.jd(2008, 1, 1, 1, 0.5)
    DateTime.jd.should == DateTime.jd(0, 0, 0, 0, 0)
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
