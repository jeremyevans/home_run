require File.expand_path('../../spec_helper', __FILE__)

ruby_version_is "1.9" do
  describe "Date#next_day" do
    it "should add a single day to a Date if no arguments" do
      (Date.civil(2007,2,27).next_day).should == Date.civil(2007, 2, 28)
    end

    it "should add the given number of days to a Date if an argument" do
      (Date.civil(2007,2,27).next_day(2)).should == Date.civil(2007, 3, 1)
    end

    it "should handle a negative argument by subtracting days" do
      (Date.civil(2007,2,27).next_day(-2)).should == Date.civil(2007, 2, 25)
    end

    it "should keep the same class as the receiver" do
      c = Class.new(Date)
      c.jd.next_day.should be_kind_of(c)
    end
  end 

  describe "Date#prev_day" do
    it "should subtract a single day to a Date if no arguments" do
      (Date.civil(2007,2,27).prev_day).should == Date.civil(2007, 2, 26)
    end

    it "should subtract the given number of days to a Date if an argument" do
      (Date.civil(2007,2,27).prev_day(2)).should == Date.civil(2007, 2, 25)
    end

    it "should handle a negative argument by adding days" do
      (Date.civil(2007,2,27).prev_day(-2)).should == Date.civil(2007, 3, 1)
    end

    it "should keep the same class as the receiver" do
      c = Class.new(Date)
      c.jd.prev_day.should be_kind_of(c)
    end
  end 

  describe "Date#next_month" do
    it "should add a single month to a Date if no arguments" do
      (Date.civil(2007,2,27).next_month).should == Date.civil(2007, 3, 27)
    end

    it "should add the given number of months to a Date if an argument" do
      (Date.civil(2007,2,27).next_month(2)).should == Date.civil(2007, 4, 27)
    end

    it "should handle a negative argument by subtracting months" do
      (Date.civil(2007,2,27).next_month(-2)).should == Date.civil(2006, 12, 27)
    end

    it "should handle adding a month where the new date is not a valid date" do
      (Date.civil(2007,1,31).next_month).should == Date.civil(2007, 2, 28)
      (Date.civil(2008,1,31).next_month).should == Date.civil(2008, 2, 29)
      (Date.civil(2007,1,31).next_month(3)).should == Date.civil(2007, 4, 30)
    end

    it "should keep the same class as the receiver" do
      c = Class.new(Date)
      c.jd.next_month.should be_kind_of(c)
    end
  end 

  describe "Date#prev_month" do
    it "should subtract a single month to a Date if no arguments" do
      (Date.civil(2007,2,27).prev_month).should == Date.civil(2007, 1, 27)
    end

    it "should subtract the given number of months to a Date if an argument" do
      (Date.civil(2007,2,27).prev_month(2)).should == Date.civil(2006, 12, 27)
    end

    it "should handle a negative argument by adding months" do
      (Date.civil(2007,2,27).prev_month(-2)).should == Date.civil(2007, 4, 27)
    end

    it "should handle subtracting a month where the new date is not a valid date" do
      (Date.civil(2007,3,31).prev_month).should == Date.civil(2007, 2, 28)
      (Date.civil(2008,3,31).prev_month).should == Date.civil(2008, 2, 29)
      (Date.civil(2007,3,31).prev_month(4)).should == Date.civil(2006, 11, 30)
    end

    it "should keep the same class as the receiver" do
      c = Class.new(Date)
      c.jd.prev_month.should be_kind_of(c)
    end
  end 

  describe "Date#next_year" do
    it "should add a single year to a Date if no arguments" do
      (Date.civil(2007,2,27).next_year).should == Date.civil(2008, 2, 27)
    end

    it "should add the given number of years to a Date if an argument" do
      (Date.civil(2007,2,27).next_year(2)).should == Date.civil(2009, 2, 27)
    end

    it "should handle a negative argument by subtracting years" do
      (Date.civil(2007,2,27).next_year(-2)).should == Date.civil(2005, 2, 27)
    end

    it "should handle adding a year where the new date is not a valid date" do
      (Date.civil(2008,2,29).next_year).should == Date.civil(2009, 2, 28)
    end

    it "should keep the same class as the receiver" do
      c = Class.new(Date)
      c.jd.next_year.should be_kind_of(c)
    end
  end 

  describe "Date#prev_year" do
    it "should add a single year to a Date if no arguments" do
      (Date.civil(2007,2,27).prev_year).should == Date.civil(2006, 2, 27)
    end

    it "should add the given number of years to a Date if an argument" do
      (Date.civil(2007,2,27).prev_year(2)).should == Date.civil(2005, 2, 27)
    end

    it "should handle a negative argument by subtracting years" do
      (Date.civil(2007,2,27).prev_year(-2)).should == Date.civil(2009, 2, 27)
    end

    it "should handle adding a year where the new date is not a valid date" do
      (Date.civil(2008,2,29).prev_year).should == Date.civil(2007, 2, 28)
    end

    it "should keep the same class as the receiver" do
      c = Class.new(Date)
      c.jd.prev_year.should be_kind_of(c)
    end
  end 
end
