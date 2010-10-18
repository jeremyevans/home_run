require File.expand_path('../../spec_helper', __FILE__)

describe :date_civil, :shared => true do
  it "creates a Date with jd 0 by default" do
    d = Date.send(@method)
    d.year.should    == -4713
    d.month.should   == 11
    d.day.should     == 24
    d.julian?.should == false
    d.jd.should      == 0
  end

  it "creates a date with arguments" do
    d = Date.send(@method, 2000, 3, 5)
    d.year.should    == 2000
    d.month.should   == 3
    d.day.should     == 5
    d.julian?.should == false
    d.jd.should      == 2451609

    # Should also work with years far in the past and future

    d = Date.send(@method, -9000, 7, 5)
    d.year.should    == -9000
    d.month.should   == 7
    d.day.should     == 5
    d.julian?.should == false
    d.jd.should      == -1565937
  
    d = Date.send(@method, 9000, 10, 14)
    d.year.should    == 9000
    d.month.should   == 10
    d.day.should     == 14
    d.julian?.should == false
    d.jd.should      == 5008529
  
  end

  it "raises errors for invalid dates" do
    lambda { Date.send(@method, 2000, 13, 31) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 2000, 12, 32) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 2000,  2, 30) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 1900,  2, 29) }.should raise_error(ArgumentError)
    lambda { Date.send(@method, 2000,  2, 29) }.should_not raise_error(ArgumentError)
  
    lambda { Date.send(@method, 1582, 10, 15) }.should_not raise_error(ArgumentError)
  
  end
end

describe "Date.civil" do
  it_behaves_like(:date_civil, :civil)
  
  it "should have defaults and an optional sg value" do
    Date.civil.should == Date.jd
    Date.civil(2008).should == Date.civil(2008, 1, 1)
    Date.civil(2008, 1).should == Date.civil(2008, 1, 1)
    Date.civil(2008, 1, 1).should == Date.civil(2008, 1, 1)
    Date.civil(2008, 1, 1, 1).should == Date.civil(2008, 1, 1)
  end

  it "should not accept more than 4 arguments" do
    proc{Date.civil(2008, 1, 1, 1, 1)}.should raise_error(ArgumentError)
  end

  it "should raise ArgumentError for invalid dates" do
    proc{Date.civil(2008, 2, 30)}.should raise_error(ArgumentError)
    proc{Date.civil(2009, 2, 29)}.should raise_error(ArgumentError)
  end

  it "should keep the same class as the receiver" do
    c = Class.new(Date)
    c.civil.should be_kind_of(c)
  end
end

describe "Date.new" do
  it_behaves_like(:date_civil, :new)
end

ruby_version_is "" ... "1.9" do
  describe "Date.new3" do
    it_behaves_like(:date_civil, :new3)
  end
end

describe "Date.valid_civil?" do

  ruby_version_is "" ... "1.9" do
    it "should be able to determine if a date is valid" do
      Date.valid_civil?(1582, 10, 14).should == Date.civil(1582, 10, 14).jd
      Date.valid_civil?(1582, 10, 15).should == Date.civil(1582, 10, 15).jd
      Date.valid_civil?(1582, 10, 14, Date::ENGLAND).should_not == nil
      Date.valid_civil?(1582, 10, 14, Date::ENGLAND).should == Date.civil(1582, 10, 14, Date::ENGLAND).jd
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

    it "should be able to handle negative months and days" do
      Date.valid_civil?(1582, -3, -31).should == Date.civil(1582, 10, 1).jd
      Date.valid_civil?(1582, -3, -28).should == Date.civil(1582, 10, 4).jd
      Date.valid_civil?(1582, -3, -27).should == Date.civil(1582, 10, 5).jd
      Date.valid_civil?(1582, -3, -22).should == Date.civil(1582, 10, 10).jd
      Date.valid_civil?(1582, -3, -21).should == Date.civil(1582, 10, 11).jd
      Date.valid_civil?(1582, -3, -18).should == Date.civil(1582, 10, 14).jd
      Date.valid_civil?(1582, -3, -17).should == Date.civil(1582, 10, 15).jd
    
      Date.valid_civil?(2007, -11, -10).should == Date.civil(2007, 2, 19).jd
      Date.valid_civil?(2008, -11, -10).should == Date.civil(2008, 2, 20).jd
    end
  end

  ruby_version_is "1.9" do
    it "should be able to determine if a date is valid" do
      Date.valid_civil?(1582, 10, 14).should == true 
      Date.valid_civil?(1582, 10, 15).should == true
      Date.valid_civil?(1582, 10, 14, Date::ENGLAND).should == true
      Date.valid_civil?(2007, 2, 24).should == true
      Date.valid_civil?(2007, 2, 24, 1).should == true
      Date.valid_civil?(2007, 2, 30, 1).should == false
    end

    it "#valid_date? should be the same as valid_civil?" do
      Date.valid_date?(2007, 2, 24, 1).should == Date.valid_civil?(2007, 2, 24, 1)
      Date.valid_date?(2007, 2, 30, 1).should == Date.valid_civil?(2007, 2, 30, 1)
    end

    it "should be able to handle negative months and days" do
      Date.valid_civil?(1582, -3, -22).should == true
      Date.valid_civil?(1582, -3, -21).should == true
      Date.valid_civil?(1582, -3, -18).should == true
      Date.valid_civil?(1582, -3, -17).should == true
    
      Date.valid_civil?(2007, -11, -10).should == true
      Date.valid_civil?(2008, -11, -10).should == true
    end
  end
end
