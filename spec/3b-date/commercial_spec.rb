require File.expand_path('../../spec_helper', __FILE__)

describe "Date#commercial" do
  ruby_version_is "" ... "1.9" do
    it "Creates a Date for the friday in the year and week given" do
      d = Date.commercial(2000, 1)
      d.year.should  == 2000
      d.month.should == 1
      d.day.should   == 7
      d.cwday.should == 5
    end
  end

  ruby_version_is "1.9" do
    it "Creates a Date for the friday in the year and week given" do
      d = Date.commercial(2000, 1)
      d.year.should  == 2000
      d.month.should == 1
      d.day.should   == 3
      d.cwday.should == 1
    end
  end

  it "Creates a Date for the correct day given the year, week and day number" do
    d = Date.commercial(2004, 1, 1)
    d.year.should   == 2003
    d.month.should  == 12
    d.day.should    == 29
    d.cwday.should  == 1
    d.cweek.should  == 1
    d.cwyear.should == 2004
  end

  it "creates only Date objects for valid weeks" do
    lambda { Date.commercial(2004, 53, 1) }.should_not raise_error(ArgumentError)
    lambda { Date.commercial(2004, 53, 0) }.should raise_error(ArgumentError)
    lambda { Date.commercial(2004, 53, 8) }.should raise_error(ArgumentError)
    lambda { Date.commercial(2004, 54, 1) }.should raise_error(ArgumentError)
    lambda { Date.commercial(2004,  0, 1) }.should raise_error(ArgumentError)

    lambda { Date.commercial(2003, 52, 1) }.should_not raise_error(ArgumentError)
    lambda { Date.commercial(2003, 53, 1) }.should raise_error(ArgumentError)
    lambda { Date.commercial(2003, 52, 0) }.should raise_error(ArgumentError)
    lambda { Date.commercial(2003, 52, 8) }.should raise_error(ArgumentError)
  end
end
