require File.expand_path('../../spec_helper', __FILE__)

ruby_version_is "1.9" do
  describe "DateTime#next_day" do
    it "should add a single day to a DateTime if no arguments" do
      (DateTime.civil(2007,2,27).next_day).should == DateTime.civil(2007, 2, 28)
    end

    it "should add the given number of days to a DateTime if an argument" do
      (DateTime.civil(2007,2,27).next_day(2)).should == DateTime.civil(2007, 3, 1)
    end

    it "should handle a negative argument by subtracting days" do
      (DateTime.civil(2007,2,27).next_day(-2)).should == DateTime.civil(2007, 2, 25)
    end

    it "should keep the same fractional part and offset" do
      (DateTime.civil(2007,2,27, 12, 0, 0, 0.5).next_day).day_fraction.should == 0.5
      (DateTime.civil(2007,2,27, 12, 0, 0, 0.5).next_day).offset.should == 0.5
    end

    it "should keep the same class as the receiver" do
      c = Class.new(DateTime)
      c.jd.next_day.should be_kind_of(c)
    end
  end 

  describe "DateTime#prev_day" do
    it "should subtract a single day to a DateTime if no arguments" do
      (DateTime.civil(2007,2,27).prev_day).should == DateTime.civil(2007, 2, 26)
    end

    it "should subtract the given number of days to a DateTime if an argument" do
      (DateTime.civil(2007,2,27).prev_day(2)).should == DateTime.civil(2007, 2, 25)
    end

    it "should handle a negative argument by adding days" do
      (DateTime.civil(2007,2,27).prev_day(-2)).should == DateTime.civil(2007, 3, 1)
    end

    it "should keep the same fractional part and offset" do
      (DateTime.civil(2007,2,27, 12, 0, 0, 0.5).prev_day).day_fraction.should == 0.5
      (DateTime.civil(2007,2,27, 12, 0, 0, 0.5).prev_day).offset.should == 0.5
    end

    it "should keep the same class as the receiver" do
      c = Class.new(DateTime)
      c.jd.prev_day.should be_kind_of(c)
    end
  end 

  describe "DateTime#next_month" do
    it "should add a single month to a DateTime if no arguments" do
      (DateTime.civil(2007,2,27).next_month).should == DateTime.civil(2007, 3, 27)
    end

    it "should add the given number of months to a DateTime if an argument" do
      (DateTime.civil(2007,2,27).next_month(2)).should == DateTime.civil(2007, 4, 27)
    end

    it "should handle a negative argument by subtracting months" do
      (DateTime.civil(2007,2,27).next_month(-2)).should == DateTime.civil(2006, 12, 27)
    end

    it "should handle adding a month where the new date is not a valid date" do
      (DateTime.civil(2007,1,31).next_month).should == DateTime.civil(2007, 2, 28)
      (DateTime.civil(2008,1,31).next_month).should == DateTime.civil(2008, 2, 29)
      (DateTime.civil(2007,1,31).next_month(3)).should == DateTime.civil(2007, 4, 30)
    end

    it "should keep the same fractional part and offset" do
      (DateTime.civil(2007,2,27, 12, 0, 0, 0.5).next_month).day_fraction.should == 0.5
      (DateTime.civil(2007,2,27, 12, 0, 0, 0.5).next_month).offset.should == 0.5
    end

    it "should keep the same class as the receiver" do
      c = Class.new(DateTime)
      c.jd.next_month.should be_kind_of(c)
    end
  end 

  describe "DateTime#prev_month" do
    it "should subtract a single month to a DateTime if no arguments" do
      (DateTime.civil(2007,2,27).prev_month).should == DateTime.civil(2007, 1, 27)
    end

    it "should subtract the given number of months to a DateTime if an argument" do
      (DateTime.civil(2007,2,27).prev_month(2)).should == DateTime.civil(2006, 12, 27)
    end

    it "should handle a negative argument by adding months" do
      (DateTime.civil(2007,2,27).prev_month(-2)).should == DateTime.civil(2007, 4, 27)
    end

    it "should handle subtracting a month where the new date is not a valid date" do
      (DateTime.civil(2007,3,31).prev_month).should == DateTime.civil(2007, 2, 28)
      (DateTime.civil(2008,3,31).prev_month).should == DateTime.civil(2008, 2, 29)
      (DateTime.civil(2007,3,31).prev_month(4)).should == DateTime.civil(2006, 11, 30)
    end

    it "should keep the same fractional part and offset" do
      (DateTime.civil(2007,2,27, 12, 0, 0, 0.5).prev_month).day_fraction.should == 0.5
      (DateTime.civil(2007,2,27, 12, 0, 0, 0.5).prev_month).offset.should == 0.5
    end

    it "should keep the same class as the receiver" do
      c = Class.new(DateTime)
      c.jd.prev_month.should be_kind_of(c)
    end
  end 

  describe "DateTime#next_year" do
    it "should add a single year to a DateTime if no arguments" do
      (DateTime.civil(2007,2,27).next_year).should == DateTime.civil(2008, 2, 27)
    end

    it "should add the given number of years to a DateTime if an argument" do
      (DateTime.civil(2007,2,27).next_year(2)).should == DateTime.civil(2009, 2, 27)
    end

    it "should handle a negative argument by subtracting years" do
      (DateTime.civil(2007,2,27).next_year(-2)).should == DateTime.civil(2005, 2, 27)
    end

    it "should handle adding a year where the new date is not a valid date" do
      (DateTime.civil(2008,2,29).next_year).should == DateTime.civil(2009, 2, 28)
    end

    it "should keep the same fractional part and offset" do
      (DateTime.civil(2007,2,27, 12, 0, 0, 0.5).next_year).day_fraction.should == 0.5
      (DateTime.civil(2007,2,27, 12, 0, 0, 0.5).next_year).offset.should == 0.5
    end

    it "should keep the same class as the receiver" do
      c = Class.new(DateTime)
      c.jd.next_year.should be_kind_of(c)
    end
  end 

  describe "DateTime#prev_year" do
    it "should add a single year to a DateTime if no arguments" do
      (DateTime.civil(2007,2,27).prev_year).should == DateTime.civil(2006, 2, 27)
    end

    it "should add the given number of years to a DateTime if an argument" do
      (DateTime.civil(2007,2,27).prev_year(2)).should == DateTime.civil(2005, 2, 27)
    end

    it "should handle a negative argument by subtracting years" do
      (DateTime.civil(2007,2,27).prev_year(-2)).should == DateTime.civil(2009, 2, 27)
    end

    it "should handle adding a year where the new date is not a valid date" do
      (DateTime.civil(2008,2,29).prev_year).should == DateTime.civil(2007, 2, 28)
    end

    it "should keep the same fractional part and offset" do
      (DateTime.civil(2007,2,27, 12, 0, 0, 0.5).prev_year).day_fraction.should == 0.5
      (DateTime.civil(2007,2,27, 12, 0, 0, 0.5).prev_year).offset.should == 0.5
    end

    it "should keep the same class as the receiver" do
      c = Class.new(DateTime)
      c.jd.prev_year.should be_kind_of(c)
    end
  end 

end
